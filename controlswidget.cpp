#include "controlswidget.h"
#include "ui_controlswidget.h"

ControlsWidget::ControlsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlsWidget)
{
    ui->setupUi(this);

    initialized = false;
}

ControlsWidget::~ControlsWidget()
{
    delete ui;
}

void ControlsWidget::initialize(Controls *controls){
    this->controls = controls;

    connect(ui->scrambleKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onScrambleKeySequenceChanged()));
    connect(ui->resetKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onResetKeySequenceChanged()));
    connect(ui->increaseSizeKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onIncreaseSizeKeySequenceChanged()));
    connect(ui->decreaseSizeKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onDecreaseSizeKeySequenceChanged()));
    connect(ui->changeSizeKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onChangeSizeKeySequenceChanged()));

    connect(ui->loadProjection1KeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onLoadProjection1KeySequenceChanged()));
    connect(ui->loadProjection2KeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onLoadProjection2KeySequenceChanged()));
    connect(ui->loadProjection3KeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onLoadProjection3KeySequenceChanged()));
    connect(ui->loadProjection4KeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onLoadProjection4KeySequenceChanged()));
    connect(ui->loadProjection5KeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onLoadProjection5KeySequenceChanged()));
    connect(ui->loadProjection6KeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onLoadProjection6KeySequenceChanged()));
    connect(ui->loadProjection7KeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onLoadProjection7KeySequenceChanged()));
    connect(ui->loadProjection8KeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onLoadProjection8KeySequenceChanged()));
    connect(ui->loadProjection9KeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onLoadProjection9KeySequenceChanged()));
    connect(ui->loadProjection10KeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onLoadProjection10KeySequenceChanged()));
    connect(ui->resetProjectionKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onResetProjectionKeySequenceChanged()));
    connect(ui->changeProjectionKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onChangeProjectionKeySequenceChanged()));

    connect(ui->resetZoomKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onResetZoomKeySequenceChanged()));
    connect(ui->zoomInKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onZoomInKeySequenceChanged()));
    connect(ui->zoomInSmallKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onZoomInSmallKeySequenceChanged()));
    connect(ui->zoomInLargeKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onZoomInLargeKeySequenceChanged()));
    connect(ui->zoomOutKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onZoomOutKeySequenceChanged()));
    connect(ui->zoomOutSmallKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onZoomOutSmallKeySequenceChanged()));
    connect(ui->zoomOutLargeKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onZoomOutLargeKeySequenceChanged()));

    connect(ui->saveKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onSaveKeySequenceChanged()));
    connect(ui->loadKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onLoadKeySequenceChanged()));

    connect(ui->toggleStatsKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onToggleStatsKeySequenceChanged()));

    connect(ui->toggleMultisliceKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onToggleMultisliceKeySequenceChanged()));

    initialized = true;
}

void ControlsWidget::onScrambleKeySequenceChanged(){
    controls->setScrambleShortcutKeySequence(ui->scrambleKeySequenceEdit->keySequence());
}

void ControlsWidget::onResetKeySequenceChanged(){
    controls->setResetShortcutKeySequence(ui->resetKeySequenceEdit->keySequence());
}

void ControlsWidget::onIncreaseSizeKeySequenceChanged(){
    controls->setIncreaseSizeShortcutKeySequence(ui->increaseSizeKeySequenceEdit->keySequence());
}

void ControlsWidget::onDecreaseSizeKeySequenceChanged(){
    controls->setDecreaseSizeShortcutKeySequence(ui->decreaseSizeKeySequenceEdit->keySequence());
}

void ControlsWidget::onChangeSizeKeySequenceChanged(){
    controls->setChangeSizeShortcutKeySequence(ui->changeSizeKeySequenceEdit->keySequence());
}

void ControlsWidget::onLoadProjection1KeySequenceChanged(){
    controls->setLoadProjectionShortcutKeySequence(1, ui->loadProjection1KeySequenceEdit->keySequence());
}

void ControlsWidget::onLoadProjection2KeySequenceChanged(){
    controls->setLoadProjectionShortcutKeySequence(2, ui->loadProjection2KeySequenceEdit->keySequence());
}

void ControlsWidget::onLoadProjection3KeySequenceChanged(){
    controls->setLoadProjectionShortcutKeySequence(3, ui->loadProjection3KeySequenceEdit->keySequence());
}

void ControlsWidget::onLoadProjection4KeySequenceChanged(){
    controls->setLoadProjectionShortcutKeySequence(4, ui->loadProjection4KeySequenceEdit->keySequence());
}

void ControlsWidget::onLoadProjection5KeySequenceChanged(){
    controls->setLoadProjectionShortcutKeySequence(5, ui->loadProjection5KeySequenceEdit->keySequence());
}

void ControlsWidget::onLoadProjection6KeySequenceChanged(){
    controls->setLoadProjectionShortcutKeySequence(6, ui->loadProjection6KeySequenceEdit->keySequence());
}

void ControlsWidget::onLoadProjection7KeySequenceChanged(){
    controls->setLoadProjectionShortcutKeySequence(7, ui->loadProjection7KeySequenceEdit->keySequence());
}

void ControlsWidget::onLoadProjection8KeySequenceChanged(){
    controls->setLoadProjectionShortcutKeySequence(8, ui->loadProjection8KeySequenceEdit->keySequence());
}

void ControlsWidget::onLoadProjection9KeySequenceChanged(){
    controls->setLoadProjectionShortcutKeySequence(9, ui->loadProjection9KeySequenceEdit->keySequence());
}

void ControlsWidget::onLoadProjection10KeySequenceChanged(){
    controls->setLoadProjectionShortcutKeySequence(10, ui->loadProjection10KeySequenceEdit->keySequence());
}

void ControlsWidget::onResetProjectionKeySequenceChanged(){
    controls->setResetProjectionShortcutKeySequence(ui->resetProjectionKeySequenceEdit->keySequence());
}

void ControlsWidget::onChangeProjectionKeySequenceChanged(){
    controls->setChangeProjectionShortcutKeySequence(ui->changeProjectionKeySequenceEdit->keySequence());
}

void ControlsWidget::onResetZoomKeySequenceChanged(){
    controls->setResetZoomShortcutKeySequence(ui->resetZoomKeySequenceEdit->keySequence());
}

void ControlsWidget::onZoomInKeySequenceChanged(){
    controls->setZoomInShortcutKeySequence(ui->zoomInKeySequenceEdit->keySequence());
}

void ControlsWidget::onZoomInSmallKeySequenceChanged(){
    controls->setZoomInSmallShortcutKeySequence(ui->zoomInSmallKeySequenceEdit->keySequence());
}

void ControlsWidget::onZoomInLargeKeySequenceChanged(){
    controls->setZoomInLargeShortcutKeySequence(ui->zoomInLargeKeySequenceEdit->keySequence());
}

void ControlsWidget::onZoomOutKeySequenceChanged(){
    controls->setZoomOutShortcutKeySequence(ui->zoomOutKeySequenceEdit->keySequence());
}

void ControlsWidget::onZoomOutSmallKeySequenceChanged(){
    controls->setZoomOutSmallShortcutKeySequence(ui->zoomOutSmallKeySequenceEdit->keySequence());
}

void ControlsWidget::onZoomOutLargeKeySequenceChanged(){
    controls->setZoomOutLargeShortcutKeySequence(ui->zoomOutLargeKeySequenceEdit->keySequence());
}

void ControlsWidget::onSaveKeySequenceChanged(){
    controls->setSaveShortcutKeySequence(ui->saveKeySequenceEdit->keySequence());
}

void ControlsWidget::onLoadKeySequenceChanged(){
    controls->setLoadShortcutKeySequence(ui->loadKeySequenceEdit->keySequence());
}

void ControlsWidget::onToggleStatsKeySequenceChanged(){
    controls->setToggleStatsShortcutKeySequence(ui->toggleStatsKeySequenceEdit->keySequence());
}

void ControlsWidget::onToggleMultisliceKeySequenceChanged(){
    controls->setToggleMultisliceShortcutKeySequence(ui->toggleMultisliceKeySequenceEdit->keySequence());
}
