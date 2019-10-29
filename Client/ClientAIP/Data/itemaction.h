#ifndef ITEMACTION_H
#define ITEMACTION_H

#include <QObject>

class QtItem;
class ItemActions;

class ItemAction : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QtItem* source READ source NOTIFY sourceChanged)
	Q_PROPERTY(QtItem* receiver READ receiver WRITE setReceiver NOTIFY receiverChanged)

	Q_PROPERTY(int needValue READ needValue WRITE setNeedValue NOTIFY needValueChanged)
	Q_PROPERTY(int setValue READ setValue WRITE setSetValue NOTIFY setValueChanged)
public:

	explicit ItemAction(ItemActions *parent = nullptr);

	void trigger();

	QtItem* source();
	void setSource(QtItem *source);

	QtItem* receiver();
	void setReceiver(QtItem *receiver);

	int needValue() const;
	void setNeedValue(int needValue);

	int setValue() const;
	void setSetValue(int setValue);

signals:
	void sourceChanged();
	void receiverChanged();
	void needValueChanged();
	void setValueChanged();

public slots:
	void deleteAction();

private:
	int m_needValue;
	int m_setValue;
	QtItem *m_source;
	QtItem *m_receiver;
	ItemActions *m_itemActions;

};

#endif // ITEMACTION_H
