#ifndef CONTROLERLIST_H
#define CONTROLERLIST_H

#include "Objects/controler.h"

class ControlerList : public QObject
{
	Q_OBJECT
public:
	explicit ControlerList(QObject *parent = nullptr);
	~ControlerList();

	Controler *fromKey(const QString &key);
signals:

public slots:
	void addControler(Controler *controler);

	void removeControler(Controler *controler);

private:
	void onDisconnectedControler();

private:
	QList<Controler *> m_controlers;
};

#endif // CONTROLERLIST_H
