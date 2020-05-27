#include "controls.h"

Controls::Controls(QWidget *parent) : QObject(parent)
{
    scrambleShortcut = new QShortcut(parent);
    resetShortcut = new QShortcut(parent);
    increaseSizeShortcut = new QShortcut(parent);
    decreaseSizeShortcut = new QShortcut(parent);
    changeSizeShortcut = new QShortcut(parent);

    for(int i=0; i<10; i++){
        loadProjectionShortcuts.append(new QShortcut(parent));
    }
    resetProjectionShortcut = new QShortcut(parent);
    changeProjectionShortcut = new QShortcut(parent);

    resetZoomShortcut = new QShortcut(parent);
    zoomInShortcut = new QShortcut(parent);
    zoomInSmallShortcut = new QShortcut(parent);
    zoomInLargeShortcut = new QShortcut(parent);
    zoomOutShortcut = new QShortcut(parent);
    zoomOutSmallShortcut = new QShortcut(parent);
    zoomOutLargeShortcut = new QShortcut(parent);

    saveShortcut = new QShortcut(parent);
    loadShortcut = new QShortcut(parent);

    toggleStatsShortcut = new QShortcut(parent);

    toggleMultisliceShortcut = new QShortcut(parent);

    screenshotShortcut = new QShortcut(parent);

    connect(scrambleShortcut, SIGNAL(activated()), this, SIGNAL(scrambleShortcutActivated()));
    connect(resetShortcut, SIGNAL(activated()), this, SIGNAL(resetShortcutActivated()));
    connect(increaseSizeShortcut, SIGNAL(activated()), this, SIGNAL(increaseSizeShortcutActivated()));
    connect(decreaseSizeShortcut, SIGNAL(activated()), this, SIGNAL(decreaseSizeShortcutActivated()));
    connect(changeSizeShortcut, SIGNAL(activated()), this, SIGNAL(changeSizeShortcutActivated()));

    for(int i=0; i<10; i++){
        connect(loadProjectionShortcuts[i], SIGNAL(activated()), this, SLOT(onLoadProjectionShortcutActivated()));
    }
    connect(resetProjectionShortcut, SIGNAL(activated()), this, SIGNAL(resetProjectionShortcutActivated()));
    connect(changeProjectionShortcut, SIGNAL(activated()), this, SIGNAL(changeProjectionShortcutActivated()));

    connect(resetZoomShortcut, SIGNAL(activated()), this, SIGNAL(resetZoomShortcutActivated()));
    connect(zoomInShortcut, SIGNAL(activated()), this, SIGNAL(zoomInShortcutActivated()));
    connect(zoomInSmallShortcut, SIGNAL(activated()), this, SIGNAL(zoomInSmallShortcutActivated()));
    connect(zoomInLargeShortcut, SIGNAL(activated()), this, SIGNAL(zoomInLargeShortcutActivated()));
    connect(zoomOutShortcut, SIGNAL(activated()), this, SIGNAL(zoomOutShortcutActivated()));
    connect(zoomOutSmallShortcut, SIGNAL(activated()), this, SIGNAL(zoomOutSmallShortcutActivated()));
    connect(zoomOutLargeShortcut, SIGNAL(activated()), this, SIGNAL(zoomOutLargeShortcutActivated()));

    connect(saveShortcut, SIGNAL(activated()), this, SIGNAL(saveShortcutActivated()));
    connect(loadShortcut, SIGNAL(activated()), this, SIGNAL(loadShortcutActivated()));

    connect(toggleStatsShortcut, SIGNAL(activated()), this, SIGNAL(toggleStatsShortcutActivated()));

    connect(toggleMultisliceShortcut, SIGNAL(activated()), this, SIGNAL(toggleMultisliceShortcutActivated()));
    connect(screenshotShortcut, SIGNAL(activated()), this, SIGNAL(screenshotShortcutActivated()));
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
}

void Controls::setResetShortcutKeySequence(QKeySequence k){
    resetShortcut->setKey(k);
}

void Controls::setIncreaseSizeShortcutKeySequence(QKeySequence k){
    increaseSizeShortcut->setKey(k);
}

void Controls::setDecreaseSizeShortcutKeySequence(QKeySequence k){
    decreaseSizeShortcut->setKey(k);
}

void Controls::setChangeSizeShortcutKeySequence(QKeySequence k){
    changeSizeShortcut->setKey(k);
}

void Controls::setLoadProjectionShortcutKeySequence(int i, QKeySequence k){
    assert(1 <= i && i <= 10);
    loadProjectionShortcuts[i-1]->setKey(k);
}

void Controls::setResetProjectionShortcutKeySequence(QKeySequence k){
    resetProjectionShortcut->setKey(k);
}

void Controls::setChangeProjectionShortcutKeySequence(QKeySequence k){
    changeProjectionShortcut->setKey(k);
}

void Controls::setResetZoomShortcutKeySequence(QKeySequence k){
    resetZoomShortcut->setKey(k);
}

void Controls::setZoomInShortcutKeySequence(QKeySequence k){
    zoomInShortcut->setKey(k);
}

void Controls::setZoomInSmallShortcutKeySequence(QKeySequence k){
    zoomInSmallShortcut->setKey(k);
}

void Controls::setZoomInLargeShortcutKeySequence(QKeySequence k){
    zoomInLargeShortcut->setKey(k);
}

void Controls::setZoomOutShortcutKeySequence(QKeySequence k){
    zoomOutShortcut->setKey(k);
}

void Controls::setZoomOutSmallShortcutKeySequence(QKeySequence k){
    zoomOutSmallShortcut->setKey(k);
}

void Controls::setZoomOutLargeShortcutKeySequence(QKeySequence k){
    zoomOutLargeShortcut->setKey(k);
}

void Controls::setSaveShortcutKeySequence(QKeySequence k){
    saveShortcut->setKey(k);
}

void Controls::setLoadShortcutKeySequence(QKeySequence k){
    loadShortcut->setKey(k);
}

void Controls::setToggleStatsShortcutKeySequence(QKeySequence k){
    toggleStatsShortcut->setKey(k);
}

void Controls::setToggleMultisliceShortcutKeySequence(QKeySequence k){
    toggleMultisliceShortcut->setKey(k);
}

void Controls::setScreenshotShortcutKeySequence(QKeySequence k){
    screenshotShortcut->setKey(k);
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
}
