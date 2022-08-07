#include "audioengine.h"

#include <QMidiFile.h>
#include <QMidiOut.h>
#include <QTimer>
#include <QElapsedTimer>
#include <QVariantAnimation>

struct AudioEnginePrivate {
    QMidiFile midiFile;
    QMidiOut midiOut;

    AudioEngine::State state = AudioEngine::State::PreGame;

    QList<AudioTrack*> tracks;
    int tracksDone = 0;
};

AudioEngine::AudioEngine(QObject* parent)
    : QObject{parent} {
    d = new AudioEnginePrivate();

    d->midiFile.load(":/Overworld.mid");

    auto midi = QMidiOut::devices();
    d->midiOut.connect(QMidiOut::devices().keys().first());

    for (auto track : d->midiFile.tracks()) {
//        if (track != 0) continue;
        auto audioTrack = new AudioTrack(&d->midiFile, &d->midiOut, track, this);
        connect(audioTrack, &AudioTrack::done, this, &AudioEngine::trackDone);
        d->tracks.append(audioTrack);
    }

    d->tracks.at(0)->setVolume(0);
    d->tracks.at(1)->setVolume(0);
    d->tracks.at(2)->setVolume(0.7);
    d->tracks.at(3)->setVolume(0.7);
    d->tracks.at(4)->setVolume(0);
}

AudioEngine::~AudioEngine() {
    d->midiOut.stopAll();
    delete d;
}

void AudioEngine::trackDone() {
    d->tracksDone++;
    if (d->tracks.length() == d->tracksDone) {
        for (auto track : d->tracks) {
            track->restart();
        }
        d->tracksDone = 0;
    }
}

void AudioEngine::setState(State state) {
    if (d->state == state) return;

    switch (state) {
        case State::PreGame:
            this->animateTrackVolume(0, 0);
            this->animateTrackVolume(1, 0);
            this->animateTrackVolume(2, 0.7);
            this->animateTrackVolume(3, 0.7);
            this->animateTrackVolume(4, 0);
            break;
        case State::Game5H:
            this->animateTrackVolume(0, 1);
            this->animateTrackVolume(1, 1);
            this->animateTrackVolume(2, 0.7);
            this->animateTrackVolume(3, 0.7);
            this->animateTrackVolume(4, 0);
            break;
        case State::EndGame:
            this->animateTrackVolume(0, 0);
            this->animateTrackVolume(1, 0);
            this->animateTrackVolume(2, 0);
            this->animateTrackVolume(3, 0.7);
            this->animateTrackVolume(4, 0);
            break;
        case State::GameDanger:
            this->animateTrackVolume(0, 1);
            this->animateTrackVolume(1, 1);
            this->animateTrackVolume(2, 0.7);
            this->animateTrackVolume(3, 0.7);
            this->animateTrackVolume(4, 0.7);
            break;
    }
    d->state = state;
}

void AudioEngine::animateTrackVolume(int track, double to) {
    auto anim = new QVariantAnimation();
    anim->setStartValue(d->tracks.at(track)->volume());
    anim->setEndValue(to);
    anim->setDuration(1000);
    connect(anim, &QVariantAnimation::valueChanged, this, [this, track](QVariant value) {
        d->tracks.at(track)->setVolume(value.toDouble());
    });
    connect(anim, &QVariantAnimation::finished, anim, &QVariantAnimation::deleteLater);
    anim->start();
}

struct AudioTrackPrivate {
    QElapsedTimer time;
    int track;
    double volume = 1;
    QMidiFile* file;
    QMidiOut* midiOut;

    QList<QMidiEvent*> events;
    QList<QMidiEvent*>::iterator eventIterator;
};

AudioTrack::AudioTrack(QMidiFile* file, QMidiOut* out, int track, QObject* parent) : QObject(parent) {
    d = new AudioTrackPrivate();
    d->events =  file->eventsForTrack(track);
    d->file = file;
    d->midiOut = out;

    d->eventIterator = d->events.begin();
    d->time.restart();
    this->consumeEvent();
}

AudioTrack::~AudioTrack() {
    delete d;
}

void AudioTrack::restart() {
    d->eventIterator = d->events.begin();
    d->time.restart();
    this->consumeEvent();
}

void AudioTrack::consumeEvent() {
    if (d->eventIterator == d->events.end()) {
        emit done();
        return;
    }
    d->eventIterator++;
    if (d->eventIterator == d->events.end()) {
        emit done();
        return;
    }

    auto event = *d->eventIterator;
    if (event->type() != QMidiEvent::Meta) {
        auto time = d->file->timeFromTick(event->tick()) * 1000;
        auto waitTime = d->time.elapsed();
        QTimer::singleShot(qMax<int>(0, time - waitTime), this, [this, event] {
            if (event->type() == QMidiEvent::SysEx) {

            } else {
                auto newEvent = QMidiEvent(*event);
//                if (newEvent.track() > 1) newEvent.setVelocity(newEvent.velocity() * 0.5);
                newEvent.setVelocity(newEvent.velocity() * d->volume);
                d->midiOut->sendEvent(newEvent);
            }
            this->consumeEvent();
        });
    } else {
        this->consumeEvent();
    }
}

void AudioTrack::setVolume(double volume) {
    d->volume = volume;
}

double AudioTrack::volume() {
    return d->volume;
}
