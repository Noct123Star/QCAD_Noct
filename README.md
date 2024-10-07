# GIT_QCAD

## **获取项目：**

- 分支branch：master分支为未经修改的原始文件，实现功能分支的为"**保存-另存-读取-新建**"

- 在visual studio中克隆项目：链接在"**<>Code**"中获取，即为 https://github.com/Noct123Star/QCAD_Noct.git    若用克隆的方式获取项目，需要在打开项目后，在最上栏的:  **Git--管理分支**中，将实现功能的分支**签出**
- 直接下载项目的ZIP。注意下载时，所在分支branch不是**master**而是实现功能的"**保存-另存-读取-新建**"分支

## 1-4项基本功能实现描述

### 保存

在`mainwindow.cpp`中的`saveFile()`中编写相关代码。

1.添加保存的代码：

保存当前图元数量。针对EntityList中的每个图元，保存当前图元的类型EntityType，调用`MEntity`类的`Serialize(QDataStream& ar, bool bSave)`方法将EntityList中的各个图元对象串行化保存。由于使用 QDataStream 类数据流，因而数据保存结果为二进制文件。

```c++
void MainWindow::saveFile(){
    // 保存为
    int EntityNum = view->GetEntityList().size();
    out << EntityNum;
    EEntityType EnType;
    foreach(MEntity * pEnt, view->GetEntityList()) {
        EnType = (EEntityType)pEnt->GetType();
        out << EnType;
        switch (EnType) {
        case EEntityType::etLine:
            MLine* line = (MLine*)pEnt;
            line->Serialize(out, true);
            break;
        }
    }
    std::cout << "文档保存成功." << '\n';
}
```

2.添加保存格式选择：`.cad`

```c++
void MainWindow::saveFile(){
    if (m_sFileName == tr("untitled"))
	{
        QString sName = QFileDialog::getSaveFileName(this, tr("Save Picture"), "", "CAD(*.cad)");
        if (sName.isEmpty())
        {
            return;
        }
        m_sFileName = sName;
	}
}
```

3.修改ENTITY.CPP中的`Serialize(QDataStream& ar, bool bSave)`代码：

代码if-else执行后的代码存在未初始化的变量被赋值的情况，直接调用会报错。所以将后段的赋值操作的代码置入else(即反串行化的读取内容)中。

```c++
void MEntity::Serialize(QDataStream& ar, bool bSave)
{
	int			 lineWidth;
	Qt::PenStyle lineStyle;
	int intStyle;
	QColor  	color;

	if (bSave)
		ar << m_pen.color() << m_pen.style() << m_pen.width();
	else
	{
		ar >> color >> intStyle >> lineWidth;

		lineStyle = (Qt::PenStyle)intStyle;
		m_pen = QPen(color);
		m_pen.setWidth(lineWidth);
		m_pen.setStyle(lineStyle);
	}
}
```

*******

### 另存

在`mainwindow.cpp`中的`saveAsFile()`中编写相关代码。

代码内容与`saveFile()`基本一致，但减少了检查文档命名的if判定，直接提示输入文件名并选择保存的位置。

```c++
void MainWindow::saveAsFile()
{
    //提示输入文件名
    QString sName = QFileDialog::getSaveFileName(this, tr("Save Picture"), "", "CAD(*.cad)");
    if (sName.isEmpty())
    {
        return;
    }
    m_sFileName = sName;
    //将内存内容保存到文档中
    QFile file(m_sFileName);
    if (!file.open(QIODevice::ReadWrite))
    {
        qDebug() << "open file failed";
        return;
    }
    QDataStream out(&file);
    //scene->Save(out);
    int EntityNum = view->GetEntityList().size();
    out << EntityNum;
    EEntityType EnType;
    foreach(MEntity * pEnt, view->GetEntityList()) {
        EnType = (EEntityType)pEnt->GetType();
        out << EnType;
        switch (EnType) {
        case EEntityType::etLine:
            MLine* line = (MLine*)pEnt;
            line->Serialize(out, true);
            break;
        }
    }
    std::cout << "文档保存成功." << '\n';
    file.close();
    return;
}
```

******

### 读取

主要在`mainwindow.cpp`中的`readFile()`中编写相关代码。

1.清空内存

在QCADView.cpp中添加EntityList的清空函数`clearList()`，在`openFile()`中调用。

```c++
void QCADView::clearList()
{
	foreach(MEntity * pEnt, m_EntityList)
	{
		if (pEnt)
		{
			m_EntityList.removeOne(pEnt);
			delete pEnt;
			pEnt = nullptr;
			//qDebug() << "EntityList size--" << m_EntityList.size();
		}
	}
	m_EntityList.clear();
}
```

2.读取文件

依据上述各个数据在`saveFile()`中的保存顺序，依次读取图元数、图元类型、图元的成员等内容。

```c++
void MainWindow::openFile()
{
    int nCount;
    in >> nCount;
    for (int i = 0; i < nCount; i++) {
        int EnType;
        MLine* line;
        in >> EnType;
        switch (EnType)
        {
        case EEntityType::etLine:
            line = new MLine();
            line->Serialize(in, false);
            view->addEntity(line);
            break;
        }
    }
    std::cout << "文档读取成功." << '\n';
}
```

3.检查当前有没有文档，且发生更改

参考"新建"中的相同内容

****

### 新建

在`mainwindow.cpp`中的`newFile()`中编写相关代码。

1.检查当前有没有文档，且发生更改

对当前文档是否具有路径以及EntityList是否为空判定，判定不通过则调用窗口提示: "保存/丢弃/取消"，选择保存则调用保存函数。

```c++
if (!m_sFileName.isEmpty() && view->GetEntityList().size() != 0) {
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("QCAD"),
        tr("The file has been changed.\n"
            "Do you want to save?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        saveFile(); // 调用保存函数
        break;
    case QMessageBox::Cancel:
        return;
    default: // Discard
        break;
    }
}
```

2.在新建后的窗口的左上栏显示文件名:`"QCAD - untitled.cad`

```c++
m_sFileName = tr("untitled.cad");
setWindowTitle(tr("QCAD - %1").arg(m_sFileName));
```

## 文件被修改标记，退出提醒保存
### 设置修改标志
1. 在`QCADView.h`中添加`m_isModified`成员变量，并在构造函数中初始化为`false`。

```c++
private:
    bool m_isModified;
```

```c++
QCADView::QCADView()
{
    m_isModified = false;
}
```
2. 构造修改变量函数SetModifiedFlag()以及查询变量函数isModified()。
```c++
void QCADView::SetModifiedFlag(bool bModified)
{
	m_isModified = bModified;
	if (bModified) 
	{
		// 遍历m_EntityList  
		for (auto& pEntity : m_EntityList) 
		{
			MEntity* pShowEntity = pEntity->GetCurrentEnt();
			if (pShowEntity != nullptr) 
			{
				MEntity* pNext = pShowEntity->next;
				while (pNext) 
				{
					MEntity* pTmp = pNext;
					pNext = pNext->next;
					delete pTmp; // 手动删除，假设没有使用智能指针  
				}
				pShowEntity->next = nullptr;
			}
			else 
			{
				pEntity->m_nOperationNum = 0;
			}
		}
		m_nCurrentOperation++;
		m_nOperations = m_nCurrentOperation;
	}
}

bool QCADView::isModified() const
{
	return m_isModified;
}
```
### 对应操作位置设置修改标志
在`mainwindow.cpp`中，对新建、打开、保存、另存等操作位置设置修改标志。
``` c++
void MainWindow::newFile()
{
    ...
    // 设置未修改
    view->SetModifiedFlag(false);
}

void MainWindow::openFile()
{
    ...
    // 设置未修改
    view->SetModifiedFlag(false);
}

void MainWindow::saveFile()
{
    ...
    // 设置未修改
    view->SetModifiedFlag(false);
}

void MainWindow::saveAsFile()
{
    ...
    // 设置未修改
    view->SetModifiedFlag(false);
}
```
在`QCADView.cpp`中，对绘制直线操作设置修改标志
``` c++
void QCADView::drawLine()
{
	//QPainter pDC(this);

	if (m_pCmd && m_pCmd->GetType() == ctCreateLine)
		return;

	delete m_pCmd;
	m_pCmd = new MCreateLine(this);
	if (m_pCmd != nullptr)
	{
		SetModifiedFlag(true);
	}
}
```
### 重写退出操作槽函数以及closeEvent函数
重定义退出操作的槽函数`exitEvent()`
```c++
void MainWindow::createActions()
{
    ...
    exitAction = new QAction(QIcon("images/exit.png"), tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitWindow()));
}
void MainWindow::exitWindow()
{
    if (view->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("QCAD"),
            tr("The file has been changed.\n"
                "Do you want to save?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret) {
        case QMessageBox::Save:
            saveFile(); // 调用保存函数
            QWidget::close();
            break;
        case QMessageBox::Cancel:
            QWidget::close();
            break;
        default: // Discard
            break;
        }
    }
    else
    {
        QWidget::close();
    }
}
```
重定义`closeEvent()`函数
```c++
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (view->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("QCAD"),
            tr("The file has been changed.\n"
                "Do you want to save?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret) {
        case QMessageBox::Save:
            saveFile(); // 调用保存函数
            QWidget::close();
            break;
        case QMessageBox::Cancel:
            QWidget::close();
            break;
        default: // Discard
            break;
        }
    }
    else
    {
        QWidget::close();
    }
}
```
