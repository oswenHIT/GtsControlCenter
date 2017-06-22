#ifndef SELECTEIMAGE_H
#define SELECTEIMAGE_H

#include <QWidget>
#include <map>
#include <qmessagebox.h>
#include "ui_selecteimage.h"

class SelecteImage : public QWidget
{
	Q_OBJECT

public:
	SelecteImage(QWidget *parent = 0);
	~SelecteImage();
	void addListItem(QString itemName);
	void deleteItem(QString item);
	int curRowNum(){ return ui.listWidget->count(); }

	public slots:
	void getClickedItemName(QListWidgetItem *item);

signals:
	void sendClickedItem(QString name);

private:
	Ui::SelecteImage ui;
	std::map<QString, QListWidgetItem*> itemList;
	int cnt;
};

#endif // SELECTEIMAGE_H
