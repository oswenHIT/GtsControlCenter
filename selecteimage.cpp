#include "selecteimage.h"

SelecteImage::SelecteImage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.listWidget, &QListWidget::itemClicked, 
			this, &SelecteImage::getClickedItemName);
}

SelecteImage::~SelecteImage()
{

}

void SelecteImage::addListItem(QString itemName)
{
	QListWidgetItem *item = new QListWidgetItem(itemName, ui.listWidget);
	itemList[itemName] = item;
}

void SelecteImage::deleteItem(QString itemName)
{
	std::map<QString, QListWidgetItem*>::iterator iter = itemList.begin();

	while (iter != itemList.end())
	{
		if (iter->first == itemName)
		{
			int row = ui.listWidget->row(iter->second);
			ui.listWidget->takeItem(row);

			delete iter->second;
			itemList.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
}

void SelecteImage::getClickedItemName(QListWidgetItem *item)
{
	std::map<QString, QListWidgetItem*>::iterator iter = itemList.begin();

	while (iter != itemList.end())
	{
		if (iter->second == item)
		{
			emit sendClickedItem(iter->first);
		}

		++iter;
	}
}