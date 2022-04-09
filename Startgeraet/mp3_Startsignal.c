//Stand 11.09.2017

#include <ao/ao.h>
#include <mpg123.h>
#include "daten.h"
#include "mp3_Startsignal.h"


int startsignal()
{
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;
    
    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;
  
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    mpg123_open(mh, "/home/pi/Startgeraet/Startsignal_lang.mp3");
    
    mpg123_getformat(mh, &rate, &channels, &encoding);

    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;  
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    do{
        //retval = mpg123_read(mh, buffer, buffer_size,&done);
        if(done>0)ao_play(dev, buffer, done);
    }while ((mpg123_read(mh, buffer, buffer_size,&done)) == MPG123_OK);

    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

    delay(1000);                //Achtung dieses delay beim Startsignal beachten!!!
    
    return 0;
}
