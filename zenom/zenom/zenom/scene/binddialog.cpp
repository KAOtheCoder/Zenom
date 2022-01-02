#include "binddialog.h"
#include "logvariableconverter.h"

#include <QVBoxLayout>
#include <QScrollArea>

BindDialog::BindDialog(const LogVariableList& pLogVariables, QWidget* pParent)
    : QDialog(pParent),
      mLogVariables(pLogVariables),
      mPropertyTracker(NULL),
      mTypeLabel(new QLabel()),
      mNameLabel(new QLabel()),
      mLogVariablesButtonGroup(new QButtonGroup(this)),
      mBindButton(new QPushButton(tr("Bind")))
{
    setWindowTitle(tr("Bind"));

    auto mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    auto propertyLayout = new QHBoxLayout();

    auto typePalette = mTypeLabel->palette();
    typePalette.setColor(mTypeLabel->foregroundRole(), QColor(Qt::darkYellow));
    mTypeLabel->setPalette(typePalette);

    auto namePalette = mNameLabel->palette();
    namePalette.setColor(mNameLabel->foregroundRole(), QColor(Qt::darkRed));
    mNameLabel->setPalette(namePalette);

    propertyLayout->addWidget(mTypeLabel);
    propertyLayout->addWidget(mNameLabel);
    propertyLayout->addStretch();
    mainLayout->addLayout(propertyLayout);

    auto logVariablesWidget = new QWidget();
    auto logVariablesLayout = new QVBoxLayout();
    logVariablesWidget->setLayout(logVariablesLayout);

    auto scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setBackgroundRole(QPalette::Light);
    scrollArea->setWidget(logVariablesWidget);
    mainLayout->addWidget(scrollArea);

    const int LOG_VARIABLES_SIZE = pLogVariables.size();

    for (int i = 0; i < LOG_VARIABLES_SIZE; ++i)
    {
        auto logVariableRadioButton = new QRadioButton(QString::fromStdString(pLogVariables[i]->name()));
        logVariablesLayout->addWidget(logVariableRadioButton);
        mLogVariablesButtonGroup->addButton(logVariableRadioButton, i);
    }

    auto buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    mBindButton->setEnabled(false);
    buttonsLayout->addWidget(mBindButton);
    auto unbindButton = new QPushButton(tr("Unbind"));
    buttonsLayout->addWidget(unbindButton);
    mainLayout->addLayout(buttonsLayout);

    connect(mBindButton, SIGNAL(clicked()), this, SLOT(bind()));
    connect(unbindButton, SIGNAL(clicked()), this, SLOT(unbind()));
    connect(mLogVariablesButtonGroup, &QButtonGroup::buttonToggled, this, &BindDialog::updateBindButtonEnabled);
}

void BindDialog::openDialog(PropertyTracker* pPropertyTracker)
{
    mPropertyTracker = pPropertyTracker;

    const auto& property = pPropertyTracker->property();

    mTypeLabel->setText(property.typeName());
    mNameLabel->setText(property.name());

    const int logVariablesSize = mLogVariables.size();

    for (int i = 0; i < logVariablesSize; ++i)
        mLogVariablesButtonGroup->button(i)->setEnabled(pPropertyTracker->canBind(mLogVariables[i]));

    updateBindButtonEnabled();

    open();
}

void BindDialog::bind()
{
    close();
    emit bindTriggered(mPropertyTracker, mLogVariables[mLogVariablesButtonGroup->checkedId()]);
}

void BindDialog::unbind()
{
    close();
    emit unbindTriggered(mPropertyTracker);
}

void BindDialog::updateBindButtonEnabled()
{
    const int checkedId = mLogVariablesButtonGroup->checkedId();

    mBindButton->setEnabled(checkedId != -1 && mLogVariablesButtonGroup->button(checkedId)->isEnabled());
}
