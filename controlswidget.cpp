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

    connect(ui->screenshotKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onScreenshotKeySequenceChanged()));

    connect(ui->settingsWindowKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onSettingsWindowKeySequenceChanged()));
    connect(ui->replayRecorderWindowKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onReplayRecorderWindowKeySequenceChanged()));
    connect(ui->reconstructionWindowKeySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(onReconstructionWindowKeySequenceChanged()));

    connect(ui->leftClickComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLeftClickComboBoxChanged()));
    connect(ui->ctrlClickComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCtrlClickComboBoxChanged()));
    connect(ui->shiftClickComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onShiftClickComboBoxChanged()));
    connect(ui->rightClickComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onRightClickComboBoxChanged()));
    connect(ui->middleClickComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onMiddleClickComboBoxChanged()));

    synchronizeFromControls();

    initialized = true;
}

void ControlsWidget::synchronizeFromControls(){
    //see SettingsWindow::synchronizeFromSettings

    QList<QWidget*> widgets = {
        ui->scrambleKeySequenceEdit,
        ui->resetKeySequenceEdit,
        ui->increaseSizeKeySequenceEdit,
        ui->decreaseSizeKeySequenceEdit,
        ui->changeSizeKeySequenceEdit,
        ui->loadProjection1KeySequenceEdit,
        ui->loadProjection2KeySequenceEdit,
        ui->loadProjection3KeySequenceEdit,
        ui->loadProjection4KeySequenceEdit,
        ui->loadProjection5KeySequenceEdit,
        ui->loadProjection6KeySequenceEdit,
        ui->loadProjection7KeySequenceEdit,
        ui->loadProjection8KeySequenceEdit,
        ui->loadProjection9KeySequenceEdit,
        ui->loadProjection10KeySequenceEdit,
        ui->resetProjectionKeySequenceEdit,
        ui->changeProjectionKeySequenceEdit,
        ui->resetZoomKeySequenceEdit,
        ui->zoomInKeySequenceEdit,
        ui->zoomInSmallKeySequenceEdit,
        ui->zoomInLargeKeySequenceEdit,
        ui->zoomOutKeySequenceEdit,
        ui->zoomOutSmallKeySequenceEdit,
        ui->zoomOutLargeKeySequenceEdit,
        ui->saveKeySequenceEdit,
        ui->loadKeySequenceEdit,
        ui->toggleStatsKeySequenceEdit,
        ui->toggleMultisliceKeySequenceEdit,
        ui->screenshotKeySequenceEdit,
        ui->leftClickComboBox,
        ui->ctrlClickComboBox,
        ui->shiftClickComboBox,
        ui->rightClickComboBox,
        ui->middleClickComboBox
    };

    foreach(QWidget *w, widgets){
        w->blockSignals(true);
    }

    ui->scrambleKeySequenceEdit->setKeySequence(controls->getScrambleShortcutKeySequence());
    ui->resetKeySequenceEdit->setKeySequence(controls->getResetShortcutKeySequence());
    ui->increaseSizeKeySequenceEdit->setKeySequence(controls->getIncreaseSizeShortcutKeySequence());
    ui->decreaseSizeKeySequenceEdit->setKeySequence(controls->getDecreaseSizeShortcutKeySequence());
    ui->changeSizeKeySequenceEdit->setKeySequence(controls->getChangeSizeShortcutKeySequence());

    ui->loadProjection1KeySequenceEdit->setKeySequence(controls->getLoadProjectionShortcutKeySequence(1));
    ui->loadProjection2KeySequenceEdit->setKeySequence(controls->getLoadProjectionShortcutKeySequence(2));
    ui->loadProjection3KeySequenceEdit->setKeySequence(controls->getLoadProjectionShortcutKeySequence(3));
    ui->loadProjection4KeySequenceEdit->setKeySequence(controls->getLoadProjectionShortcutKeySequence(4));
    ui->loadProjection5KeySequenceEdit->setKeySequence(controls->getLoadProjectionShortcutKeySequence(5));
    ui->loadProjection6KeySequenceEdit->setKeySequence(controls->getLoadProjectionShortcutKeySequence(6));
    ui->loadProjection7KeySequenceEdit->setKeySequence(controls->getLoadProjectionShortcutKeySequence(7));
    ui->loadProjection8KeySequenceEdit->setKeySequence(controls->getLoadProjectionShortcutKeySequence(8));
    ui->loadProjection9KeySequenceEdit->setKeySequence(controls->getLoadProjectionShortcutKeySequence(9));
    ui->loadProjection10KeySequenceEdit->setKeySequence(controls->getLoadProjectionShortcutKeySequence(10));
    ui->resetProjectionKeySequenceEdit->setKeySequence(controls->getResetProjectionShortcutKeySequence());
    ui->changeProjectionKeySequenceEdit->setKeySequence(controls->getChangeProjectionShortcutKeySequence());

    ui->resetZoomKeySequenceEdit->setKeySequence(controls->getResetZoomShortcutKeySequence());
    ui->zoomInKeySequenceEdit->setKeySequence(controls->getZoomInShortcutKeySequence());
    ui->zoomInSmallKeySequenceEdit->setKeySequence(controls->getZoomInSmallShortcutKeySequence());
    ui->zoomInLargeKeySequenceEdit->setKeySequence(controls->getZoomInLargeShortcutKeySequence());
    ui->zoomOutKeySequenceEdit->setKeySequence(controls->getZoomOutShortcutKeySequence());
    ui->zoomOutSmallKeySequenceEdit->setKeySequence(controls->getZoomOutSmallShortcutKeySequence());
    ui->zoomOutLargeKeySequenceEdit->setKeySequence(controls->getZoomOutLargeShortcutKeySequence());

    ui->saveKeySequenceEdit->setKeySequence(controls->getSaveShortcutKeySequence());
    ui->loadKeySequenceEdit->setKeySequence(controls->getLoadShortcutKeySequence());

    ui->toggleStatsKeySequenceEdit->setKeySequence(controls->getToggleStatsShortcutKeySequence());

    ui->toggleMultisliceKeySequenceEdit->setKeySequence(controls->getToggleMultisliceShortcutKeySequence());

    ui->screenshotKeySequenceEdit->setKeySequence(controls->getScreenshotShortcutKeySequence());

    ui->leftClickComboBox->setCurrentIndex((int)controls->getLeftClickAction());
    ui->ctrlClickComboBox->setCurrentIndex((int)controls->getCtrlClickAction());
    ui->shiftClickComboBox->setCurrentIndex((int)controls->getShiftClickAction());
    ui->rightClickComboBox->setCurrentIndex((int)controls->getRightClickAction());
    ui->middleClickComboBox->setCurrentIndex((int)controls->getMiddleClickAction());

    foreach(QWidget *w, widgets){
        w->blockSignals(false);
    }
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

void ControlsWidget::onScreenshotKeySequenceChanged(){
    controls->setScreenshotShortcutKeySequence(ui->screenshotKeySequenceEdit->keySequence());
}

void ControlsWidget::onSettingsWindowKeySequenceChanged(){
    controls->setSettingsWindowShortcutKeySequence(ui->settingsWindowKeySequenceEdit->keySequence());
}

void ControlsWidget::onReplayRecorderWindowKeySequenceChanged(){
    controls->setReplayRecorderWindowShortcutKeySequence(ui->replayRecorderWindowKeySequenceEdit->keySequence());
}

void ControlsWidget::onReconstructionWindowKeySequenceChanged(){
    controls->setReconstructionWindowShortcutKeySequence(ui->reconstructionWindowKeySequenceEdit->keySequence());
}

void ControlsWidget::onLeftClickComboBoxChanged(){
    controls->setLeftClickAction((MoveType)ui->leftClickComboBox->currentIndex());
}

void ControlsWidget::onCtrlClickComboBoxChanged(){
    controls->setCtrlClickAction((MoveType)ui->ctrlClickComboBox->currentIndex());
}

void ControlsWidget::onShiftClickComboBoxChanged(){
    controls->setShiftClickAction((MoveType)ui->shiftClickComboBox->currentIndex());
}

void ControlsWidget::onRightClickComboBoxChanged(){
    controls->setRightClickAction((MoveType)ui->rightClickComboBox->currentIndex());
}

void ControlsWidget::onMiddleClickComboBoxChanged(){
    controls->setMiddleClickAction((MoveType)ui->middleClickComboBox->currentIndex());
}
