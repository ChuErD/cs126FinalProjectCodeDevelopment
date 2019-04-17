#include "ofApp.h"
#include <vector>
#include <math.h>


//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground( 0 );

    std::string filepath = ofToDataPath("redemption_song.wav"); // mono original file
	std::string test_filepath = ofToDataPath("frozen.wav");
    //std::string filepath = ofToDataPath("tubophone.mp3"); // at the moment mp3 isn't working on ARM
    //std::string filepath = ofToDataPath("tubophone.ogg"); // ogg/vorbis
    //std::string filepath = ofToDataPath("tubophone.flac"); // FLAC

    audiofile.setVerbose(true); 
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    if( ofFile::doesFileExist( filepath ) ){
        audiofile.load( filepath );
        if (!audiofile.loaded()){
            ofLogError()<<"error loading file, double check the file path";
        }
    }else{
        ofLogError()<<"input file does not exists";
    }

	if (ofFile::doesFileExist(test_filepath)) {
		test_audiofile.load(test_filepath);
		if (!test_audiofile.loaded()) {
			ofLogError() << "error loading file, double check the file path";
		}
	}
	else {
		ofLogError() << "input file does not exists";
	}
    
    // audio setup for testing audio file stream 
	ofSoundStreamSettings settings;
    sampleRate = 44100.0;
    settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = 512;
	ofSoundStreamSetup(settings);

	//test_audiofile setup
	ofSoundStreamSettings test_settings;
	test_sampleRate = 44100.0;
	test_settings.setOutListener(this);
	test_settings.sampleRate = sampleRate;
	test_settings.numOutputChannels = 2;
	test_settings.numInputChannels = 0;
	test_settings.bufferSize = 512;
	ofSoundStreamSetup(settings);

	// audiofile setup
    playhead = std::numeric_limits<int>::max(); // because it is converted to int for check
    playheadControl = -1.0;
    step = audiofile.samplerate() / sampleRate;
	std::cout << "step size: " << step << endl;

	// test_audiofile setup
	test_playhead = std::numeric_limits<int>::max(); // because it is converted to int for check
	test_playheadControl = -1.0;
	test_step = test_audiofile.samplerate() / test_sampleRate;
	std::cout << "test step size: " << step << endl;

	// --------------------------------------------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------------------------------------------
	// code block to calculate test_audiofile means
	std::vector<std::vector<float>> total_y;
	for (int c = 0; c < test_audiofile.channels(); ++c) {

		std::vector<float> current_channel_total_y;

		float max = ofGetWidth();
		for (int x = 0; x < max; ++x) {
			int n = ofMap(x, 0, max, 0, test_audiofile.length(), true);
			float val = test_audiofile.sample(n, c);
			// float y = ofMap(val, -1.0f, 1.0f, ofGetHeight()*0.5, 0.0f); // used to be 0.5 instead of 0.25, 1.0 instead of 2.0
			current_channel_total_y.push_back(val);
		}

		total_y.push_back(current_channel_total_y);
	}

	test_audiofile_channel_0_mean = getMean(total_y[0]);

	test_audiofile_channel_1_mean = getMean(total_y[1]);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	std::vector<std::vector<float>> total_y;
    
    ofNoFill();
    
    ofPushMatrix();
    for( int c=0; c<audiofile.channels(); ++c ){

		std::vector<float> current_channel_total_y;
        
        float max = ofGetWidth();
        ofBeginShape();
        for( int x=0; x<max; ++x){
            int n = ofMap( x, 0, max, 0, audiofile.length(), true );
            float val = audiofile.sample( n, c );
            float y = ofMap( val, -1.0f, 1.0f, ofGetHeight()*0.5, 0.0f ); // used to be 0.5 instead of 0.25, 1.0 instead of 2.0
			current_channel_total_y.push_back(val);
            ofVertex( x, y );
        }
        ofEndShape();        
  
        float phx = ofMap( playhead, 0, audiofile.length(), 0, max );
        ofDrawLine( phx, 0, phx, ofGetHeight()*0.5f); // used to be 0.5
        
        ofTranslate( 0.0, ofGetHeight()*0.5 ); // used to be 0.5

		total_y.push_back(current_channel_total_y);
    }
    ofPopMatrix();

	float channel_0_mean = getMean(total_y[0]);

	float channel_1_mean = getMean(total_y[1]);

	float channel_0_std = getStd(total_y[0], channel_0_mean);

	float channel_1_std = getStd(total_y[1], channel_1_mean);
    
    ofDrawBitmapString( audiofile.path(), 10, 20 );

	float to_draw_distance_channel_0 = (test_audiofile_channel_0_mean - channel_0_mean) / (channel_0_std);

	float to_draw_distance_channel_1 = (test_audiofile_channel_1_mean - channel_1_mean) / (channel_1_std);

	ofDrawBitmapString(to_draw_distance_channel_0, ofGetWidth() / 2, ofGetHeight() - 30);

	ofDrawBitmapString(to_draw_distance_channel_1, ofGetWidth() / 2, ofGetHeight() - 15);

	ofDrawBitmapString("hey oh, press SPACEBAR to play, press L to load a sample", ofGetWidth() / 2 - 225, ofGetHeight() - 50);

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
    
    // really spartan and not efficient sample playing, just for testing
    
    if( playheadControl >= 0.0 ){
        playhead = playheadControl;
        playheadControl = -1.0;
    }
    
    for (size_t i = 0; i < buffer.getNumFrames(); i++){
        
        int n = playhead;
        
        if( n < audiofile.length()-1 ){
            
            for( size_t k=0; k<buffer.getNumChannels(); ++k){
                if( k < audiofile.channels() ){
                    float fract = playhead - (double) n;
                    float s0 = audiofile.sample( n, k );
                    float s1 = audiofile.sample( n+1, k );
                    float isample = s0*(1.0-fract) + s1*fract; // linear interpolation
                    buffer[i*buffer.getNumChannels() + k] = isample;
                }else{
                    buffer[i*buffer.getNumChannels() + k] = 0.0f;
                }
            }
            
            playhead += step;
        
        }else{
            buffer[i*buffer.getNumChannels()    ] = 0.0f;
            buffer[i*buffer.getNumChannels() + 1] = 0.0f;
            playhead = std::numeric_limits<int>::max();
        }

    }
}

float ofApp::getMean(std::vector<float>& vect)
{
	float sum = 0.0;
	for (int i = 0; i < vect.size(); ++i) {
		sum += vect[i];
	}
	float to_return = (sum) / (vect.size());
	return to_return;
}

float ofApp::getStd(std::vector<float>& vect, float mean)
{
	float summed_diff = 0.0;
	for (int i = 0; i < vect.size(); ++i) {
		summed_diff += pow((vect[i] - mean), 2);
	}
	summed_diff /= vect.size();
	return sqrt(summed_diff);
}
		
		

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    audiofile.load( dragInfo.files[0] );
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofSoundStreamClose();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == ' ') playheadControl = 0.0;
    
    if( key == 'l' || key=='L'){
       //Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("select an audio sample"); 
        //Check if the user opened a file
        if (openFileResult.bSuccess){
            string filepath = openFileResult.getPath();            
            audiofile.load ( filepath );
            step = audiofile.samplerate() / sampleRate;
            ofLogVerbose("file loaded");
        }else {
            ofLogVerbose("User hit cancel");
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}
