#pragma once


#include "ofMain.h"
#include "ofxAudioFile.h"
#include <vector>
#include <string>


class ofApp: public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
        void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void audioOut(ofSoundBuffer & buffer);

		float getMean(std::vector<float> &vect);

		float getStd(std::vector<float> &vect, float mean);
        
		// -----------------------BASE SONG VARIABLES----------------------- //
        ofxAudioFile audiofile;
		std::vector<std::vector<float>> base_song_data;

		// Audio out
		double playhead;
		std::atomic<double> playheadControl;
		double step;
		double sampleRate;

		// Mean and std for base song
		float audiofile_channel_0_mean;
		float audiofile_channel_1_mean;
		float audiofile_channel_0_std;
		float audiofile_channel_1_std;

		// ---------------------TEST SONG VARIABLES-------------------------- //
		ofxAudioFile test_audiofile;
		std::vector<std::vector<float>> test_song_data;
        
		//audio out
		double test_playhead;
		std::atomic<double> test_playheadControl;
		double test_step;
		double test_sampleRate;

		// Mean and std for test song
		float test_audiofile_channel_0_mean;
		float test_audiofile_channel_1_mean;
		float test_audiofile_channel_0_std;
		float test_audiofile_channel_1_std;

		// ---------------------FIND SONG VARIABLES-------------------------- //
		ofxAudioFile find_audiofile;
		std::vector<std::vector<float>> find_song_data;

		//audio out
		double find_playhead;
		std::atomic<double> find_playheadControl;
		double find_step;
		double find_sampleRate;

		// Mean and std for test song
		float find_audiofile_channel_0_mean;
		float find_audiofile_channel_1_mean;
		float find_audiofile_channel_0_std;
		float find_audiofile_channel_1_std;

		std::string guess_output = "";
};
