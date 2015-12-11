#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"
#include "client.h"
#include "GPS.h"
#include "util.h"
#include <string.h>

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (128)
#define NUM_SECONDS     (1)
#define NUM_CHANNELS    (1)
/* Select sample format. */
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"

typedef struct
{
    int          frameIndex;  /* Index into sample array. */
    int          maxFrameIndex;
    SAMPLE      *recordedSamples;
}
paTestData;


static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    paTestData *data = (paTestData*)userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) outputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if( framesLeft < framesPerBuffer )
    {
        framesToCalc = framesLeft;
        finished = paComplete;
    }
    else
    {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if( inputBuffer == NULL )
    {
        for( i=0; i<framesToCalc; i++ )
        {
            *wptr++ = SAMPLE_SILENCE;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
        }
    }
    else
    {
        for( i=0; i<framesToCalc; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
    }
    data->frameIndex += framesToCalc;
    return finished;
}

static void processAndSend(){

            gps_info gi;
            char buffer[10] = {'\0'};
            char message[100] = {'\0'};


            const char* alert = "Gunshot! ";
            const char* lat = "Latitude:";
            const char* longitude = "Longitude:";
            const char* date = "TimeStamp:";
            const char* space = " ";

            strcat(message, alert);

            GetGPSInfo(&gi);

            itoa(gi.latitude, buffer, 10);
            strcat(message, lat);        
            strcat(message, (const char*)buffer);
            strcat(message, space);

            itoa(gi.longitude, buffer, 10);
            strcat(message, longitude);
            strcat(message, (const char*)buffer);
            strcat(message, space);

            strcat(message, date);
            strcat(message, (const char*)asctime(gi.timeinfo));

            Send(message);        

}


int main(void)
{
    PaStreamParameters inputParameters;
    PaStream* stream;
    PaError err = paNoError;
    paTestData data;
    int i, totalFrames, numSamples, numBytes;
    SAMPLE              max, val;
    double average;   
 
    // Set up connection to server
    if(Init() == -1)
        return -1;


    // do this forever
    for(;;){

        data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
        data.frameIndex = 0;
        numSamples = totalFrames * NUM_CHANNELS;
        numBytes = numSamples * sizeof(SAMPLE);
        data.recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
        if( data.recordedSamples == NULL )
        {
            printf("Could not allocate record array.\n");
            goto done;
        }

        // Zero initialize the Recorded Samples
        for(i=0; i<numSamples; i++ ) 
            data.recordedSamples[i] = 0;

        // Initialize PortAudio
        err = Pa_Initialize();

        if( err != paNoError ) 
            goto done;

        // Get input device params
        inputParameters.device = Pa_GetDefaultInputDevice();

        if (inputParameters.device == paNoDevice) {
            fprintf(stderr,"Error: No default input device.\n");
            goto done;
        }
        inputParameters.channelCount = 1;
        inputParameters.sampleFormat = PA_SAMPLE_TYPE;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
        inputParameters.hostApiSpecificStreamInfo = NULL;

        /* Capture */
        err = Pa_OpenStream(&stream, &inputParameters,NULL, SAMPLE_RATE,FRAMES_PER_BUFFER, paClipOff, recordCallback, &data );
        
        if( err != paNoError ) 
            goto done;

        err = Pa_StartStream( stream );
        
        if( err != paNoError ) 
            goto done;
        
        printf("\n=== recording!!===\n"); 
        fflush(stdout);

        while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
        {
            Pa_Sleep(100);
        }
        if( err < 0 ) 
            goto done;

        err = Pa_CloseStream( stream );
        if( err != paNoError ) 
            goto done;

        /* Measure maximum peak amplitude. */
        max = 0;
        average = 0.0;

        for( i=0; i<numSamples; i++ )
        {
            val = data.recordedSamples[i];
            if( val < 0 ) val = -val; /* ABS */
            if( val > max )
            {
                max = val;
            }
            average += val;
        }
        average = average / (double)numSamples;
        Pa_Terminate();
        
        if(max > 20000){
            processAndSend();
        }       
        if( data.recordedSamples )       /* Sure it is NULL or valid. */
            free( data.recordedSamples );
    }// for-loop

    done:
    Pa_Terminate();
    if( data.recordedSamples )       /* Sure it is NULL or valid. */
        free( data.recordedSamples );
    if( err != paNoError )
    {
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        err = 1;          /* Always return 0 or 1, but no other return codes. */
    }
    return err;
}

