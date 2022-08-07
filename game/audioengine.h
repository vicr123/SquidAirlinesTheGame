#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>

struct AudioEnginePrivate;
class AudioEngine : public QObject {
        Q_OBJECT
    public:
        explicit AudioEngine(QObject* parent = nullptr);
        ~AudioEngine();

        void trackDone();

        void startGame();
        void endGame();

    signals:

    private:
        AudioEnginePrivate* d;

};

class QMidiOut;
class QMidiFile;
class QMidiEvent;
struct AudioTrackPrivate;
class AudioTrack : public QObject {
        Q_OBJECT
    public:
        explicit AudioTrack(QMidiFile* file, QMidiOut* out, int track, QObject* parent = nullptr);
        ~AudioTrack();

        void restart();
        void consumeEvent();

        void setVolume(double volume);

    signals:
        void done();

    private:
        AudioTrackPrivate* d;
};

#endif // AUDIOENGINE_H
