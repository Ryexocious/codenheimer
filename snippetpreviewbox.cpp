#include "snippetpreviewbox.h"
#include "ui_snippetpreviewbox.h"
#include "tagviewer.h"
#include "flowlayout.h"

snippetPreviewBox::snippetPreviewBox(MainWindow *mainwindow, QWidget *parent)
    :QWidget(parent),
    ui(new Ui::snippetPreviewBox),
    masterWindow(mainwindow)
{
    ui->setupUi(this);


    //getting and setting the font
    CutiveMonoFont=masterWindow->getFont("Cutive");
    CutiveMonoFont.setPointSize(19);
    ui->Name->setFont(CutiveMonoFont);
    ui->groupBox->installEventFilter(this);
    addCopyButton();
}

snippetPreviewBox::~snippetPreviewBox()
{
    delete ui;
}

void snippetPreviewBox::assignSnippet(snippetBaseClass* snippet){
    assignedSnippet=snippet;
    tags=assignedSnippet->getInfo(name, lang);
    ui->Name->setText(QString::fromStdString(name));
    ui->langLabel->setText(QString::fromStdString(lang));
    if(assignedSnippet->isLocked()) setLock();
    setTags();
}

snippetBaseClass *snippetPreviewBox::getSnippetObj()
{
    return assignedSnippet;
}

void snippetPreviewBox::setTags(){

    FlowLayout* fl=new FlowLayout(10,3,1);
    fl->setSpacing(2);
    for(auto& str: *tags){
        std::string name="dickpbfdb";
        std::string color="red";
        masterWindow->getTagInfo(str, name, color);
        if(name=="NoTag") continue;
        tagViewer* tv=new tagViewer(this);
        tv->setTag(name, color);
        fl->addWidget(tv);
    }
    ui->tagsBox->setLayout(fl);
    // ui->tagsBox->layout()->setSpacing(2);
}

void snippetPreviewBox::setLock()
{
    //setting the icon

    /** old system using QLabel
    *
    *    // QPixmap pixmap(":/images/lockIcon.svg");
    *    // ui->lockIcon->setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    *
    */

    ui->lockIcon->setIcon(QIcon(":/images/lockIcon.svg"));
    ui->lockIcon->setIconSize(QSize(18,18));

}

void snippetPreviewBox::addCopyButton()
{
    ui->copyButton->setIcon(QIcon(":images/copyButton.svg"));
    ui->copyButton->setIconSize(QSize(18,18));
    // ui->copyButton->setStyleSheet("color:rgba(0,0,0,0);");
    ui->copyButton->hide();
}

void snippetPreviewBox::mouseDoubleClickEvent(QMouseEvent *event) {
    qDebug() << "Widget double-clicked!";
    QString nam=ui->Name->text();
    masterWindow->openSnippetInEditor(assignedSnippet, nam  , true);
    QWidget::mouseDoubleClickEvent(event);  // Call base class method
}

// Event filter implementation
bool snippetPreviewBox::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->groupBox) { // Ensure ui->groupBox is accessible
        if (event->type() == QEvent::Enter) {
            // qDebug() << "Mouse entered the group box";
            ui->copyButton->show();
            return true; // Event handled
        } else if (event->type() == QEvent::Leave) {
            // qDebug() << "Mouse left the group box";
            ui->copyButton->hide();
            return true; // Event handled
        }
    }

    // Pass unhandled events to the base class
    return QWidget::eventFilter(watched, event);
}


void snippetPreviewBox::on_copyButton_clicked()
{
    std::string str="DefaultTextFailedToReadSnippet";
    readSnippet(str);
    str=assignedSnippet->getSnippet();
    QString text=QString::fromStdString(str);
    //qDebug("the content of the std::string is=%s\nand the QString is=%s",str.c_str(),text.toStdString().c_str());
    masterWindow->copyToClipboard(text);
}

void snippetPreviewBox::readSnippet(std::string& str)
{

    //call str=assignedSnippet->getSnippet();
}

QString snippetPreviewBox::getNam(){
    QString nam=ui->Name->text();
    return nam;
}


