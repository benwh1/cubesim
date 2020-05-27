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

    zoomInShortcut = new QShortcut(parent);
    zoomInSmallShortcut = new QShortcut(parent);
    zoomOutShortcut = new QShortcut(parent);
    zoomOutSmallShortcut = new QShortcut(parent);

    saveShortcut = new QShortcut(parent);
    loadShortcut = new QShortcut(parent);

    toggleStatsShortcut = new QShortcut(parent);

    toggleMultisliceShortcut = new QShortcut(parent);

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

    connect(zoomInShortcut, SIGNAL(activated()), this, SIGNAL(zoomInShortcutActivated()));
    connect(zoomInSmallShortcut, SIGNAL(activated()), this, SIGNAL(zoomInSmallShortcutActivated()));
    connect(zoomOutShortcut, SIGNAL(activated()), this, SIGNAL(zoomOutShortcutActivated()));
    connect(zoomOutSmallShortcut, SIGNAL(activated()), this, SIGNAL(zoomOutSmallShortcutActivated()));

    connect(saveShortcut, SIGNAL(activated()), this, SIGNAL(saveShortcutActivated()));
    connect(loadShortcut, SIGNAL(activated()), this, SIGNAL(loadShortcutActivated()));

    connect(toggleStatsShortcut, SIGNAL(activated()), this, SIGNAL(toggleStatsShortcutActivated()));

    connect(toggleMultisliceShortcut, SIGNAL(activated()), this, SIGNAL(toggleMultisliceShortcutActivated()));
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

void Controls::setZoomInShortcutKeySequence(QKeySequence k){
    zoomInShortcut->setKey(k);
}

void Controls::setZoomInSmallShortcutKeySequence(QKeySequence k){
    zoomInSmallShortcut->setKey(k);
}

void Controls::setZoomOutShortcutKeySequence(QKeySequence k){
    zoomOutShortcut->setKey(k);
}

void Controls::setZoomOutSmallShortcutKeySequence(QKeySequence k){
    zoomOutSmallShortcut->setKey(k);
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
