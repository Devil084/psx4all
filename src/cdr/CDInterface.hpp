/************************************************************************

Copyright mooby 2002

CDRMooby2 CDInterface.hpp
http://mooby.psxfanatics.com

  This file is protected by the GNU GPL which should be included with
  the source code distribution.

************************************************************************/


#ifndef CDINTERFACE_HPP
#define CDINTERFACE_HPP

#include <vector>
#include <strstream>
#include <sstream>

#include "TrackInfo.hpp"
#include "SubchannelData.hpp"
#include "CDDAData.hpp"
#include "TrackParser.hpp"
#include "FileInterface.hpp"

extern TDTNFormat tdtnformat;
extern std::string programName;

// a container class for holding all the CD info needed
class CDInterface
{
public:
   CDInterface()    
      : scd(NULL),
        cdda(NULL), image(NULL)
   {
         // based on the program name, decide what order to return the
         // track info.
      programName = getProgramName();
      if ( (programName == "epsxe") ||
           (programName == "pcsx")  ||
           (programName == "pcsx2") )
         tdtnformat = fsmint;
      else
         tdtnformat = msfbcd;
   }

		// opens the file
   inline void open(const std::string& str);

		// cleans up
   ~CDInterface()
   {
      if (cdda)
         delete cdda;
      if (image)
         delete image;
      if (scd)
         delete scd;
   }

		// returns the number of tracks - 1 because trackList[0] is 
		// the full CD length
	inline unsigned long getNumTracks() const
      {return trackList.size() - 1;}

		// returns the TrackInfo for trackNum
	inline TrackInfo getTrackInfo(const unsigned long trackNum) const
      throw(Exception);

		// seeks the data pointer to time
   inline void moveDataPointer(const CDTime& time) throw(Exception)
   {
      image->seek(time); 
      scd->seek(time);
   }
		// returns the pointer to the data buffer
   inline unsigned char* readDataPointer() const 
      {return image->getBuffer();}
		// returns the pointer to the subchannel data
	inline unsigned char* readSubchannelPointer() const
      {return scd->get();}
		// returns the CD length
   inline CDTime getCDLength() {return image->getCDLength();}

		// starts CDDA playing at time
   inline int playTrack(const CDTime& time) {return cdda->play(time);}
		// stops CDDA playing
   inline int stopTrack() {return cdda->stop();}
      // returns the time of the sector that's currently being read
   inline CDTime readTime(void) 
   {
      if (cdda->isPlaying())
         return cdda->playTime();
      else
         return image->getSeekTime();
   }
      // returns whether or not the CDDA is playing
   inline bool isPlaying (void) {return cdda->isPlaying();}

private:
		// a vector of track info that stores the start, end, and length of each track
   std::vector<TrackInfo> trackList;

		// the subchannel data for the cd
   SubchannelData* scd;
		// the CDDA data for the cd
   CDDAData* cdda;
		// the interface to the CD image itself
   FileInterface* image;
};

// initalizes all the data for CDInterface based on a file name from
// CDOpen
inline void CDInterface::open(const std::string& str)
{
		// use the FIFactory to make the FileInterface
   std::string extension;
   image = FileInterfaceFactory(str, extension);

   std::string fileroot = str;
   fileroot.erase(fileroot.rfind(extension));

   TrackParser* tp = TrackParserFactory(fileroot);
   tp->parse();
   tp->postProcess(image->getCDLength());
   trackList = tp->getTrackList();
   delete tp;

		// if there is more than one track, initalize the CDDA data
   if (trackList.size() > 2)
   {
      cdda = new PlayCDDAData(trackList, tp->hasPregap());
      cdda->openFile(str);
   }
   else
   {
      cdda = new NoCDDAData();
   }

		// build the subchannel data
   scd = SubchannelDataFactory(fileroot);

   if (tp->hasPregap())
   {
      image->setPregap(true, trackList[2].trackStart);
   }
}

// returns the TrackInfo for trackNum if it exists
inline TrackInfo CDInterface::getTrackInfo(const unsigned long trackNum) const
   throw(Exception)
{
   if (trackNum >= trackList.size())
   {
      std::ostringstream ost;
      ost << trackNum << std::endl;
      Exception e(std::string("Track number out of bounds") + ost.str());
      THROW(e);
   }
   return trackList[trackNum];
}

#endif
