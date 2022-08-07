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

        enum class State {
            PreGame,
            Game5H,
            GameDanger,
            EndGame
        };

        void setState(State state);

    signals:

    private:
        AudioEnginePrivate* d;

        void animateTrackVolume(int track, double to);
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
        double volume();

    signals:
        void done();

    private:
        AudioTrackPrivate* d;
};

#endif // AUDIOENGINE_H
