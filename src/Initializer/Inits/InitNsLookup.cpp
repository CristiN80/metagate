﻿#include "InitNsLookup.h"

#include "../Initializer.h"

#include "NsLookup.h"

#include "check.h"
#include "SlotWrapper.h"

namespace initializer {

InitNsLookup::InitNsLookup(QThread *mainThread, Initializer &manager)
    : QObject(nullptr)
    , InitInterface(mainThread, manager)
{}

InitNsLookup::~InitNsLookup() = default;

void InitNsLookup::complete() {
    CHECK(nsLookup != nullptr, "nsLookup not initialized");
    CHECK(isFlushed, "Not flushed");
}

void InitNsLookup::sendInitSuccess(const TypedException &exception) {
    sendState(InitState("nslookup", "init", "nslookup initialized", exception));
}

void InitNsLookup::onServersFlushed() {
BEGIN_SLOT_WRAPPER
    if (!isFlushed) {
        sendState(InitState("nslookup", "flushed", "nslookup flushed", TypedException()));
        isFlushed = true;
    }
END_SLOT_WRAPPER
}

InitNsLookup::Return InitNsLookup::initialize() {
    const TypedException exception = apiVrapper2([&, this] {
        nsLookup = std::make_unique<NsLookup>();
        CHECK(connect(nsLookup.get(), &NsLookup::serversFlushed, this, &InitNsLookup::onServersFlushed), "not connect onServersFlushed");
        nsLookup->start();
    });
    sendInitSuccess(exception);
    if (exception.isSet()) {
        throw exception;
    }
    return nsLookup.get();
}

}