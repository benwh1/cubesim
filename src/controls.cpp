#include "controls.h"

Controls::Controls(QWidget *parent) : QObject(parent)
{
    //define a temporary macro to make the shortcut creation more readable

#define F(keys) new QShortcut(QKeySequence(keys), parent)

    scrambleShortcut             = F("Space");
    resetShortcut                = F("Esc");
    increaseSizeShortcut         = F("=");
    decreaseSizeShortcut         = F("-");
    changeSizeShortcut           = F("+");
    resetProjectionShortcut      = F("D");
    changeProjectionShortcut     = F("P");
    resetZoomShortcut            = F("Home");
    zoomInShortcut               = F("PgUp");
    zoomInSmallShortcut          = F("Ctrl+PgUp");
    zoomInLargeShortcut          = F("Shift+PgUp");
    zoomOutShortcut              = F("PgDown");
    zoomOutSmallShortcut         = F("Ctrl+PgDown");
    zoomOutLargeShortcut         = F("Shift+PgDown");
    saveShortcut                 = F("Ctrl+S");
    loadShortcut                 = F("Ctrl+O");
    toggleStatsShortcut          = F("V");
    toggleMultisliceShortcut     = F("CapsLock");
    screenshotShortcut           = F("Ctrl+P");
    settingsWindowShortcut       = F("W");
    replayRecorderWindowShortcut = F("R");
    reconstructionWindowShortcut = F("L");

    for(int i=1; i<=10; i++){
        QShortcut *s = F(QString::number(i%10));
        loadProjectionShortcuts.append(s);
    }

#undef F

    leftClickAction   = MoveType::QuarterTurn;
    ctrlClickAction   = MoveType::Rotation;
    shiftClickAction  = MoveType::HalfTurn;
    rightClickAction  = MoveType::HalfTurn;
    middleClickAction = MoveType::Rotation;

    //define a macro to make the signal-slot connections more readable

#define F(shortcut) connect(shortcut, SIGNAL(activated()), this, SIGNAL(shortcut##Activated()))

    F(scrambleShortcut);
    F(resetShortcut);
    F(increaseSizeShortcut);
    F(decreaseSizeShortcut);
    F(changeSizeShortcut);
    F(resetProjectionShortcut);
    F(changeProjectionShortcut);
    F(resetZoomShortcut);
    F(zoomInShortcut);
    F(zoomInSmallShortcut);
    F(zoomInLargeShortcut);
    F(zoomOutShortcut);
    F(zoomOutSmallShortcut);
    F(zoomOutLargeShortcut);
    F(saveShortcut);
    F(loadShortcut);
    F(toggleStatsShortcut);
    F(toggleMultisliceShortcut);
    F(screenshotShortcut);
    F(settingsWindowShortcut);
    F(replayRecorderWindowShortcut);
    F(reconstructionWindowShortcut);

    for(int i=0; i<10; i++){
        connect(loadProjectionShortcuts[i], SIGNAL(activated()), this, SLOT(onLoadProjectionShortcutActivated()));
    }

#undef F

}

QKeySequence Controls::getScrambleShortcutKeySequence(){
    return scrambleShortcut->key();
}

QKeySequence Controls::getResetShortcutKeySequence(){
    return resetShortcut->key();
}

QKeySequence Controls::getIncreaseSizeShortcutKeySequence(){
    return increaseSizeShortcut->key();
}

QKeySequence Controls::getDecreaseSizeShortcutKeySequence(){
    return decreaseSizeShortcut->key();
}

QKeySequence Controls::getChangeSizeShortcutKeySequence(){
    return changeSizeShortcut->key();
}

QKeySequence Controls::getLoadProjectionShortcutKeySequence(int i){
    assert(1 <= i && i <= 10);
    return loadProjectionShortcuts[i-1]->key();
}

QKeySequence Controls::getResetProjectionShortcutKeySequence(){
    return resetProjectionShortcut->key();
}

QKeySequence Controls::getChangeProjectionShortcutKeySequence(){
    return changeProjectionShortcut->key();
}

QKeySequence Controls::getResetZoomShortcutKeySequence(){
    return resetZoomShortcut->key();
}

QKeySequence Controls::getZoomInShortcutKeySequence(){
    return zoomInShortcut->key();
}

QKeySequence Controls::getZoomInSmallShortcutKeySequence(){
    return zoomInSmallShortcut->key();
}

QKeySequence Controls::getZoomInLargeShortcutKeySequence(){
    return zoomInLargeShortcut->key();
}

QKeySequence Controls::getZoomOutShortcutKeySequence(){
    return zoomOutShortcut->key();
}

QKeySequence Controls::getZoomOutSmallShortcutKeySequence(){
    return zoomOutSmallShortcut->key();
}

QKeySequence Controls::getZoomOutLargeShortcutKeySequence(){
    return zoomOutLargeShortcut->key();
}

QKeySequence Controls::getSaveShortcutKeySequence(){
    return saveShortcut->key();
}

QKeySequence Controls::getLoadShortcutKeySequence(){
    return loadShortcut->key();
}

QKeySequence Controls::getToggleStatsShortcutKeySequence(){
    return toggleStatsShortcut->key();
}

QKeySequence Controls::getToggleMultisliceShortcutKeySequence(){
    return toggleMultisliceShortcut->key();
}

QKeySequence Controls::getScreenshotShortcutKeySequence(){
    return screenshotShortcut->key();
}

QKeySequence Controls::getSettingsWindowShortcutKeySequence(){
    return settingsWindowShortcut->key();
}

QKeySequence Controls::getReplayRecorderWindowShortcutKeySequence(){
    return replayRecorderWindowShortcut->key();
}

QKeySequence Controls::getReconstructionWindowShortcutKeySequence(){
    return reconstructionWindowShortcut->key();
}

MoveType Controls::getLeftClickAction(){
    return leftClickAction;
}

MoveType Controls::getCtrlClickAction(){
    return ctrlClickAction;
}

MoveType Controls::getShiftClickAction(){
    return shiftClickAction;
}

MoveType Controls::getRightClickAction(){
    return rightClickAction;
}

MoveType Controls::getMiddleClickAction(){
    return middleClickAction;
}

void Controls::onLoadProjectionShortcutActivated(){
    QShortcut *shortcut = qobject_cast<QShortcut*>(QObject::sender());

    if(shortcut == nullptr){
        return;
    }

    //find the position of the shortcut in the list to know which
    //projection shortcut was activated
    int i = loadProjectionShortcuts.indexOf(shortcut);

    if(i == -1){
        return;
    }

    emit loadProjectionShortcutActivated(i+1);
}

void Controls::setScrambleShortcutKeySequence(QKeySequence k){
    scrambleShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setResetShortcutKeySequence(QKeySequence k){
    resetShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setIncreaseSizeShortcutKeySequence(QKeySequence k){
    increaseSizeShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setDecreaseSizeShortcutKeySequence(QKeySequence k){
    decreaseSizeShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setChangeSizeShortcutKeySequence(QKeySequence k){
    changeSizeShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setLoadProjectionShortcutKeySequence(int i, QKeySequence k){
    assert(1 <= i && i <= 10);
    loadProjectionShortcuts[i-1]->setKey(k);
    emit settingChanged();
}

void Controls::setResetProjectionShortcutKeySequence(QKeySequence k){
    resetProjectionShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setChangeProjectionShortcutKeySequence(QKeySequence k){
    changeProjectionShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setResetZoomShortcutKeySequence(QKeySequence k){
    resetZoomShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setZoomInShortcutKeySequence(QKeySequence k){
    zoomInShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setZoomInSmallShortcutKeySequence(QKeySequence k){
    zoomInSmallShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setZoomInLargeShortcutKeySequence(QKeySequence k){
    zoomInLargeShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setZoomOutShortcutKeySequence(QKeySequence k){
    zoomOutShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setZoomOutSmallShortcutKeySequence(QKeySequence k){
    zoomOutSmallShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setZoomOutLargeShortcutKeySequence(QKeySequence k){
    zoomOutLargeShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setSaveShortcutKeySequence(QKeySequence k){
    saveShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setLoadShortcutKeySequence(QKeySequence k){
    loadShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setToggleStatsShortcutKeySequence(QKeySequence k){
    toggleStatsShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setToggleMultisliceShortcutKeySequence(QKeySequence k){
    toggleMultisliceShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setScreenshotShortcutKeySequence(QKeySequence k){
    screenshotShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setSettingsWindowShortcutKeySequence(QKeySequence k){
    settingsWindowShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setReplayRecorderWindowShortcutKeySequence(QKeySequence k){
    replayRecorderWindowShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setReconstructionWindowShortcutKeySequence(QKeySequence k){
    reconstructionWindowShortcut->setKey(k);
    emit settingChanged();
}

void Controls::setLeftClickAction(MoveType m){
    leftClickAction = m;
    emit settingChanged();
}

void Controls::setCtrlClickAction(MoveType m){
    ctrlClickAction = m;
    emit settingChanged();
}

void Controls::setShiftClickAction(MoveType m){
    shiftClickAction = m;
    emit settingChanged();
}

void Controls::setRightClickAction(MoveType m){
    rightClickAction = m;
    emit settingChanged();
}

void Controls::setMiddleClickAction(MoveType m){
    middleClickAction = m;
    emit settingChanged();
}

QJsonObject Controls::toJSON(){
    QJsonObject data;

    data["scrambleShortcut"] = scrambleShortcut->key().toString();
    data["resetShortcut"] = resetShortcut->key().toString();
    data["increaseSizeShortcut"] = increaseSizeShortcut->key().toString();
    data["decreaseSizeShortcut"] = decreaseSizeShortcut->key().toString();
    data["changeSizeShortcut"] = changeSizeShortcut->key().toString();

    QJsonArray arr;
    for(int i=0; i<10; i++){
        arr.append(loadProjectionShortcuts[i]->key().toString());
    }
    data["loadProjectionShortcuts"] = arr;
    data["resetProjectionShortcut"] = resetProjectionShortcut->key().toString();
    data["changeProjectionShortcut"] = changeProjectionShortcut->key().toString();

    data["resetZoomShortcut"] = resetZoomShortcut->key().toString();
    data["zoomInShortcut"] = zoomInShortcut->key().toString();
    data["zoomInSmallShortcut"] = zoomInSmallShortcut->key().toString();
    data["zoomInLargeShortcut"] = zoomInLargeShortcut->key().toString();
    data["zoomOutShortcut"] = zoomOutShortcut->key().toString();
    data["zoomOutSmallShortcut"] = zoomOutSmallShortcut->key().toString();
    data["zoomOutLargeShortcut"] = zoomOutLargeShortcut->key().toString();

    data["saveShortcut"] = saveShortcut->key().toString();
    data["loadShortcut"] = loadShortcut->key().toString();

    data["toggleStatsShortcut"] = toggleStatsShortcut->key().toString();

    data["toggleMultisliceShortcut"] = toggleMultisliceShortcut->key().toString();

    data["screenshotShortcut"] = screenshotShortcut->key().toString();

    data["settingsWindowShortcut"] = settingsWindowShortcut->key().toString();
    data["replayRecorderWindowShortcut"] = replayRecorderWindowShortcut->key().toString();
    data["reconstructionWindowShortcut"] = reconstructionWindowShortcut->key().toString();

    data["leftClickAction"] = (int)leftClickAction;
    data["ctrlClickAction"] = (int)ctrlClickAction;
    data["shiftClickAction"] = (int)shiftClickAction;
    data["rightClickAction"] = (int)rightClickAction;
    data["middleClickAction"] = (int)middleClickAction;

    return data;
}

void Controls::fromJSON(QJsonObject data){
    scrambleShortcut->setKey(QKeySequence::fromString(data["scrambleShortcut"].toString()));
    resetShortcut->setKey(QKeySequence::fromString(data["resetShortcut"].toString()));
    increaseSizeShortcut->setKey(QKeySequence::fromString(data["increaseSizeShortcut"].toString()));
    decreaseSizeShortcut->setKey(QKeySequence::fromString(data["decreaseSizeShortcut"].toString()));
    changeSizeShortcut->setKey(QKeySequence::fromString(data["changeSizeShortcut"].toString()));

    QJsonArray arr = data["loadProjectionShortcuts"].toArray();
    //if arr.size() is not 10 for some reason, then arr[i] will not crash,
    //it will just return QJsonValue(undefined) and the shortcuts will
    //be set to the blank string, i.e. no shortcut defined
    for(int i=0; i<10; i++){
        loadProjectionShortcuts[i]->setKey(QKeySequence::fromString(arr[i].toString()));
    }
    resetProjectionShortcut->setKey(QKeySequence::fromString(data["resetProjectionShortcut"].toString()));
    changeProjectionShortcut->setKey(QKeySequence::fromString(data["changeProjectionShortcut"].toString()));

    resetZoomShortcut->setKey(QKeySequence::fromString(data["resetZoomShortcut"].toString()));
    zoomInShortcut->setKey(QKeySequence::fromString(data["zoomInShortcut"].toString()));
    zoomInSmallShortcut->setKey(QKeySequence::fromString(data["zoomInSmallShortcut"].toString()));
    zoomInLargeShortcut->setKey(QKeySequence::fromString(data["zoomInLargeShortcut"].toString()));
    zoomOutShortcut->setKey(QKeySequence::fromString(data["zoomOutShortcut"].toString()));
    zoomOutSmallShortcut->setKey(QKeySequence::fromString(data["zoomOutSmallShortcut"].toString()));
    zoomOutLargeShortcut->setKey(QKeySequence::fromString(data["zoomOutLargeShortcut"].toString()));

    saveShortcut->setKey(QKeySequence::fromString(data["saveShortcut"].toString()));
    loadShortcut->setKey(QKeySequence::fromString(data["loadShortcut"].toString()));

    toggleStatsShortcut->setKey(QKeySequence::fromString(data["toggleStatsShortcut"].toString()));

    toggleMultisliceShortcut->setKey(QKeySequence::fromString(data["toggleMultisliceShortcut"].toString()));

    screenshotShortcut->setKey(QKeySequence::fromString(data["screenshotShortcut"].toString()));

    settingsWindowShortcut->setKey(QKeySequence::fromString(data["settingsWindowShortcut"].toString()));
    replayRecorderWindowShortcut->setKey(QKeySequence::fromString(data["replayRecorderWindowShortcut"].toString()));
    reconstructionWindowShortcut->setKey(QKeySequence::fromString(data["reconstructionWindowShortcut"].toString()));

    leftClickAction = (MoveType)data["leftClickAction"].toInt();
    ctrlClickAction = (MoveType)data["ctrlClickAction"].toInt();
    shiftClickAction = (MoveType)data["shiftClickAction"].toInt();
    rightClickAction = (MoveType)data["rightClickAction"].toInt();
    middleClickAction = (MoveType)data["middleClickAction"].toInt();

    emit settingChanged();
}
