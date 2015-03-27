#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    // INIT OSC SENDER
    sender1.setup("192.168.5.90", 1234);
    sender2.setup("192.168.5.97", 1234);
    sender3.setup("192.168.5.98", 1234);
    
    // INIT OSC BROADCAST SENDER
    //sender.setup("192.168.5.255", 12340);
    
    // INIT OSC RECEIVER
    receiver.setup(1235);
    
    ofSetFrameRate(30);
    
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = ofGetWidth()-xInit;
    
    ofBackground(200, 200, 200);
        
    // MAIN GUI
    gui = new ofxUICanvas(0,0,ofGetWidth(),ofGetHeight());
    gui->setTheme(OFX_UI_THEME_MINBLACK);
    
    gui->addWidgetDown(new ofxUILabel("REMOTE 360 PETITE SALLE", OFX_UI_FONT_LARGE));
    gui->addSpacer();
    gui->addSpacer();
    gui->addWidgetDown(new ofxUILabel("PROJECTEURS", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUILabelButton("ALLUMER", false, length-xInit));
    gui->addWidgetDown(new ofxUILabelButton("ETEINDRE", false, length-xInit));
    gui->addSpacer();
    gui->addWidgetDown(new ofxUILabel("VIDEOS", OFX_UI_FONT_MEDIUM));
    
    gui->addWidgetDown(new ofxUILabelButton("0", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("1", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("2", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("3", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("4", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("5", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("6", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("7", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("8", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("9", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("10", false, 30, 30));
    
    gui->addWidgetDown(new ofxUILabelButton("PLAY", false, length-xInit));
    gui->addWidgetDown(new ofxUILabelButton("PAUSE", false, length-xInit));
    gui->addWidgetDown(new ofxUILabelButton("REMBOBINER", false, length-xInit));
    gui->addSpacer();
    gui->addSlider("TIMELINE", 0.0, 1.0, &slider_position, length-xInit, 32);
    gui->addSpacer();
    gui->addLabel("'i' : afficher les informations de configuration");
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    // HELP GUI
    helpgui = new ofxUICanvas(0,0,ofGetWidth(),ofGetHeight());
    helpgui->setTheme(OFX_UI_THEME_MINBLACK);
    helpgui->setVisible(false);
    helpgui->addWidgetDown(new ofxUILabel("INFORMATIONS", OFX_UI_FONT_LARGE));
    helpgui->addSpacer(ofGetWidth() - 5, 5);
    helpgui->addLabel("// OSC SENDERS //");
    helpgui->addLabel("PLAY : /master/play 1");
    helpgui->addLabel("PAUSE : /master/play 0");
    helpgui->addLabel("FULLSCREEN : /master/fullscreen 1");
    helpgui->addLabel("LOAD VIDEOS : /master/load_videos 1");
    helpgui->addLabel("TIMELINE : /master/slider_position (float)");
    helpgui->addLabel("Default OSC sending port : 1234");
    helpgui->addSpacer();
    helpgui->addLabel("// OSC RECEIVERS //");
    helpgui->addLabel("TIMELINE : /master/feedback (float)");
    helpgui->addSpacer();
    helpgui->addLabel("Broadcast OSC sender 192.168.5.255:12340");
    helpgui->addLabel("Default OSC receive port : 1235");
}

//--------------------------------------------------------------
void testApp::update(){
    ofxOscMessage msgRx;
    receiver.getNextMessage( &msgRx );
    
    if ( msgRx.getAddress() == "/master/feedback" )
    {
        slider_position = msgRx.getArgAsFloat(0);
    }
    
    if(slider_position == 1) {
        ofxOscMessage m;
        m.setAddress("/master/slider_position");
        m.addFloatArg(0);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
}

////////////////////// OFXUI FUNCTIONS /////////////////////////
void testApp::exit()
{
    delete gui;
}

void testApp::sendCommand(string ipAddress, int pjPort, string command)
{
    string msgRx;
    bool connected;
    
    connected = pjClient.setup(ipAddress, pjPort);
    if(!pjClient.isConnected())
    {
        cout << "Problem connecting..." << endl;
    }
    
    else {
        pjClient.sendRaw(command);
        //		ofSleepMillis(500);
        //		msgRx = "";
        //		while (msgRx.length() < 8)
        //		{
        //			msgRx = pjClient.receiveRaw();
        //		}
    }
    pjClient.close();
}

void testApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "PLAY")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            ofxOscMessage m;
            m.setAddress("/master/play");
            m.addIntArg(1);
            
            // Send broadcast message...
            //sender.sendMessage(m);
            
            // ...or to scpecified IP
            sender1.sendMessage(m);
            sender2.sendMessage(m);
            sender3.sendMessage(m);
        }
    }
    else if(e.widget->getName() == "PAUSE")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            ofxOscMessage m;
            m.setAddress("/master/play");
            m.addIntArg(0);
            
            // Send broadcast message...
            //sender.sendMessage(m);
            
            // ...or to scpecified IP
            sender1.sendMessage(m);
            sender2.sendMessage(m);
            sender3.sendMessage(m);
        }
    }
    else if(e.widget->getName() == "0")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(0);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "1")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(1);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "2")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(2);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "3")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(3);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "4")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(4);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    
    else if(e.widget->getName() == "PLEIN ECRAN")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            ofxOscMessage m;
            m.setAddress("/master/fullscreen");
            m.addIntArg(1);
            
            // Send broadcast message...
            //sender.sendMessage(m);
            
            // ...or to scpecified IP
            sender1.sendMessage(m);
            sender2.sendMessage(m);
            sender3.sendMessage(m);
        }
    }
    else if(e.widget->getName() == "REMBOBINER")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            slider_position = 0;
             ofxOscMessage m;
            m.setAddress("/master/slider_position");
            m.addFloatArg(0);
            
            // Send broadcast message...
            //sender.sendMessage(m);
            
            // ...or to scpecified IP
            sender1.sendMessage(m);
            sender2.sendMessage(m);
            sender3.sendMessage(m);
        }
    }
    else if(e.widget->getName() == "TIMELINE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        ofxOscMessage m;
        m.setAddress("/master/slider_position");
        slider_position = slider->getScaledValue();
        m.addFloatArg(slider_position);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "ALLUMER")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            sendCommand("192.168.5.121", 1025, ":POWR 1\r");
            sendCommand("192.168.5.122", 1025, ":POWR 1\r");
            sendCommand("192.168.5.123", 1025, ":POWR 1\r");
            sendCommand("192.168.5.124", 1025, ":POWR 1\r");
            sendCommand("192.168.5.125", 1025, ":POWR 1\r");
            sendCommand("192.168.5.126", 1025, ":POWR 1\r");
        }
    }
    else if(e.widget->getName() == "ETEINDRE")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            sendCommand("192.168.5.121", 1025, ":POWR 0\r");
            sendCommand("192.168.5.122", 1025, ":POWR 0\r");
            sendCommand("192.168.5.123", 1025, ":POWR 0\r");
            sendCommand("192.168.5.124", 1025, ":POWR 0\r");
            sendCommand("192.168.5.125", 1025, ":POWR 0\r");
            sendCommand("192.168.5.126", 1025, ":POWR 0\r");
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == 'i')
    {
        gui->toggleVisible();
        helpgui->toggleVisible();
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}
