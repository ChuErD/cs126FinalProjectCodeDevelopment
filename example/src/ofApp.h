#pragma once


#include "ofMain.h"
#include "ofxAudioFile.h"
#include <vector>


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
        
        ofxAudioFile audiofile;

		ofxAudioFile test_audiofile;
        
		// Audiofile
        double playhead;
        std::atomic<double> playheadControl;
        double step;
        double sampleRate;

		//test audiofile
		double test_playhead;
		std::atomic<double> test_playheadControl;
		double test_step;
		double test_sampleRate;

		//test audiofile means
		float test_audiofile_channel_0_mean;
		float test_audiofile_channel_1_mean;
		float test_audiofile_channel_0_std;
		float test_audiofile_channel_1_std;

		//base audiofile means
		float audiofile_channel_0_mean;
		float audiofile_channel_1_mean;
		float audiofile_channel_0_std;
		float audiofile_channel_1_std;
        
		void audioOut(ofSoundBuffer & buffer);

		float getMean(std::vector<float> &vect);

		float getStd(std::vector<float> &vect, float mean);

		std::vector<std::vector<float>> test_song_data;

		std::vector<std::vector<float>> base_song_data;
        	
};
