#ifndef DATARULELINEOBJECT_H
#define DATARULELINEOBJECT_H

#include <QObject>
#include <QMap>
enum RuleDataSystem{
    BIN,DEC,HEX,ASC //Binary个示2进制 Decimal表示10进制,HEX表示16进制
};
class RuleLine : public QObject{
    public:
    QString *name = 0;//名称
    int begin;//起始位
    int length;//长度
    QString suffix;//后缀
    QMap<QString,QString> *OptionMap;//值选项集
    QString defaultVal;//默认值
    RuleDataSystem rDataSystem;//解析进制
    RuleDataSystem ValueSystem = DEC;//default DEC

    explicit RuleLine(QObject *parent = nullptr);
    ~ RuleLine();
};

class RuleObject : public QObject
{
    Q_OBJECT
public:
    explicit RuleObject(QObject *parent = nullptr);
    ~ RuleObject();

private:
  int index;
  int len = 1;//default 1
  QString *comment = 0;
  QString *label = 0;
  QList<RuleLine*> *lines = 0;


public:
  void setIndex(int index);
  int getIndex();
  void SetLabel(QString *label);
  QString* getLabel();
  void setRDataSystem(RuleDataSystem rDataSystem);
  RuleDataSystem getRDataSystem();
  void setLines(QList<RuleLine> *lines);
  QList<RuleLine*>* getLines();
  void addLine(RuleLine *line);
  void removeLine(int index);
  QString GetComment();
  void SetComment(QString *comment);
  void SetLen(int len);
  int GetLen();

signals:

public slots:
};
#endif // DATARULELINEOBJECT_H
