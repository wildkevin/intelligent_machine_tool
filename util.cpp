#include <util.h>
#include <QXmlStreamReader>
#include <iostream>
#include <QMap>
#include <QDateTime>

using namespace std;

QList<RuleObject*>* ConfigUtil::ParseXMLConfig(QFile *file){
    QList<RuleObject*> *rObjects = new QList<RuleObject*>;

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        cerr << "Error: Cannot read file " << qPrintable(file->fileName())
                  << ": " << qPrintable(file->errorString())
                  << endl;
        file->close();
        return rObjects;
    }

    QXmlStreamReader xmlReader(file);

    while(!xmlReader.atEnd()) {
        xmlReader.readNextStartElement();
OUTER:
        cout<<"<<<<<xmlReader.name() == "<<xmlReader.name().toString().toStdString()<<" tokentype"<<xmlReader.tokenString().toStdString()<<endl;
        if (xmlReader.isStartElement() && xmlReader.name() == "Pos") {
            RuleObject* rObject = new RuleObject();
            QXmlStreamAttributes posAttributes = xmlReader.attributes();
            if (posAttributes.hasAttribute("value")) {
                int index = posAttributes.value("value").toInt();
                rObject->setIndex(index);
                cout<<"rObject->index = "<<QString::number(index).toStdString()<<endl;
            }
            if (posAttributes.hasAttribute("label")) {
                QString label = posAttributes.value("label").toString();
                QString *p = new QString;
                *p = label;
                rObject->SetLabel(p);
                cout<<"rObject->label = "<<label.toLocal8Bit().toStdString()<<endl;
            }

            //解析长度
            if (posAttributes.hasAttribute("len")) {
                int len = posAttributes.value("len").toInt();
                rObject->SetLen(len);
                cout<<"rObject->len = "<<QString::number(len).toStdString()<<endl;
            }

            //读pos的子节点
            while(!xmlReader.atEnd()){
                xmlReader.readNextStartElement();
                cout<<"   --xmlReader.name() == "<<xmlReader.name().toString().toStdString()<<" tokentype"<<xmlReader.tokenString().toStdString()<<endl;
                if(xmlReader.name() != "Logic"){
                    rObjects->append(rObject);
                    goto OUTER;
                }

                if(xmlReader.isStartElement()){
                    if(xmlReader.name() == "Logic"){
                        QXmlStreamAttributes logicAttributes = xmlReader.attributes();
                        RuleLine* rl = new RuleLine();
                        if (logicAttributes.hasAttribute("begin")) {
                            int begin = logicAttributes.value("begin").toInt();
                            rl->begin = begin;
                        }
                        if (logicAttributes.hasAttribute("len")) {
                            int len = logicAttributes.value("len").toInt();
                            rl->length = len;
                        }

                        //解析值选项集
                        if (logicAttributes.hasAttribute("options")) {
                            QString optionsStr = logicAttributes.value("options").toString();
                            optionsStr.replace("\n","");
                            if(!optionsStr.isEmpty() && optionsStr.contains(":")){
                        //        QMap<QString,int> map = new QMap<QString,int>;
                                QStringList optionList = optionsStr.split(";",QString::SkipEmptyParts);//拆分并跳过空值
                                for(int i=0;i<optionList.size();i++){
                                    QString s = optionList.at(i);
                                    if(s.contains(":")){
                                        QStringList ss = s.split(":",QString::SkipEmptyParts);
                                        QString k = ss.at(0);
                                        QString v = ss.at(1);
                                        rl->OptionMap->insert(k,v);
                                    }
                                }
                            }
                        }

                        if (logicAttributes.hasAttribute("datasys")) {
                            QString datasys = logicAttributes.value("datasys").toString();

                            if(datasys == "bin"){
                                rl->rDataSystem = BIN;
                            }else if(datasys == "dec"){
                                rl->rDataSystem = DEC;
                            }else if(datasys == "hex"){
                                rl->rDataSystem = HEX;
                            }else if(datasys == "asc"){
                                rl->rDataSystem = ASC;
                            }
                        }

                        if (logicAttributes.hasAttribute("defaultValue")) {
                            QString defaultStr = logicAttributes.value("defaultValue").toString();
                            rl->defaultVal = defaultStr;
                            cout<<"rl->defalutVal = "<<defaultStr.toStdString()<<endl;
                        }

                        if (logicAttributes.hasAttribute("valuesys")) {
                            QString valuesys = logicAttributes.value("valuesys").toString();
                            if(valuesys == "bin"){
                                rl->ValueSystem=BIN;
                                rl->defaultVal = rl->defaultVal.replace(" ","");
                            }else if(valuesys == "dec"){
                                rl->ValueSystem=DEC;
                                bool ok = false;
                                rl->defaultVal.toInt(&ok);
                                if(ok){
                                    rl->defaultVal = QString::number(rl->defaultVal.toInt());
                                }
                            }else if(valuesys == "hex"){
                                rl->ValueSystem=HEX;
                                rl->defaultVal = rl->defaultVal.replace(" ","");
                                QString tmp;
                                for(int i=0;i<(rl->length*2-rl->defaultVal.length());i++){
                                    tmp.append("0");
                                }

                                rl->defaultVal = tmp.append(rl->defaultVal);
                            }else if(valuesys == "asc"){
                                rl->ValueSystem=ASC;
                            }

                            cout<<"ruleLine->valuesys = "<<valuesys.toStdString()<<endl;
                        }

                        if (logicAttributes.hasAttribute("suffix")) {
                            QString suffix = logicAttributes.value("suffix").toString();
                            rl->suffix = suffix;
                        }
                        rObject->addLine(rl);
                    }
                }
            }
        }


        if (xmlReader.hasError())
        {
            cout << "XML error: " << xmlReader.errorString().toStdString() << endl;
        }

        if (xmlReader.atEnd())
        {
            cout << "Reached end, done" << endl;
        }
    }


    file->close();
    return rObjects;
}

QList<QString>* SerialPortUtil::DeParseReadData(QList<RuleObject*> *rules, QString dataOrigin){
    int headLen = 16;
    QList<QString> *output = new QList<QString>();
    QString data1 = dataOrigin.mid(headLen);
    for(int i=0; i<rules->size(); i++){
        RuleObject *rObj = rules->at(i);
        QList<RuleLine*> *rLines = rObj->getLines();
        for(int j=0; j<rLines->size(); j++){
            RuleLine *rL = rLines->at(j);
            int len = (rL->length*2);

            RuleDataSystem valueDataSys = rL->ValueSystem;
            switch (valueDataSys) {
            case HEX:
            {
                QString hexStr = data1.left(len);

                QString val;
                for(int i=0;i<hexStr.length();i+=2){
                    val.append(hexStr.mid(i,2).toUpper());
                }

                QString tmp;
                for(int i=0;i<len-val.length();i++){
                    tmp.append("0");
                }

                output->append(tmp.append(val));
                data1 = data1.mid(len);
            }
                break;
            case DEC:
            {
                QString hexStr = data1.left(len);
                //16进制转10进制
                bool ok = false;
                int intVal = hexStr.toInt(&ok,16);
                output->append(QString::number(intVal).toUpper());
                data1 = data1.mid(len);
            }
                break;
            case ASC:
            {
                QString hexStr = data1.left(len);
                QString val;
                for(int i=0;i<hexStr.length();i+=2){
                    int intVal = hexStr.mid(i,2).toInt(nullptr,16);
                    QChar ch = (QChar)intVal;
                    val.append(QString(ch).toUpper());
                }
                output->append(val);
                data1 = data1.mid(len);
            }
                break;
            case BIN:
                break;
            default:
                break;
            }

        }
    }

    return output;
}
