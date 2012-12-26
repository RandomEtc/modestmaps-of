/*
 *  ofMemoryImage.h
 *  WebImgTextLoaderThreaded
 *
 *  Created by jesús gollonet bailén on 12/09/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _OF_MEMORY_IMAGE
#define _OF_MEMORY_IMAGE

#include "ofImage.h"




class   ofMemoryImage : public ofImage { 
	
public:
    
    // this is happening in a different so don't do texture stuff here
	void loadFromData(const unsigned char * datasource, int len){
        setUseTexture(false);
        ofBuffer buffer;
        buffer.set((const char *)datasource, len);
        loadImage(buffer);
    }
    
    void initTex(){
        setUseTexture(true);
        update();
    }
    
    
    
};

#endif
