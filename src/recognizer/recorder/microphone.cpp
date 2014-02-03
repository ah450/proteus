#include "microphone.hpp"
#include <string>

using namespace proteus::utility;


Microphone::Microphone( const double sampleRate,
                        const unsigned long framesPerBuffer ) : sampleRate_( sampleRate ),
    framesPerBuffer_( framesPerBuffer ), recording(false) {
    PaError err = Pa_Initialize();

    if ( err != paNoError ) {
        throw proteus::exceptions::PortAudio(
            std::string( "PortAudio Error Code: " ) + Pa_GetErrorText( err ) );
    }

    /******************************************************************************
     *  One input stream, 0 output streams, signed 16-bit samples, ..., userData. *
     ******************************************************************************/
    err = Pa_OpenDefDefaultStream( &stream, 1, 0, paInt16, sampleRate_,
                                   framesPerBuffer_, callbacks::capturingCallback, this );

    if ( err != paNoError ) {
        throw proteus::exceptions::PortAudio(
            std::string( "PortAudio Error Code: " ) + Pa_GetErrorText( err ) );
    }

}


Microphone::start() {
    if(!recording) {
        recording = true;
        Pa_StartStream(stream);
    }
}

Microphone::stop(){
    if(recording) {
        recording = false;
        Pa_StopStream(stream);
    }
}


Microphone::~Microphone() {
    Pa_CloseStream(stream);
    Pa_Terminate();
}