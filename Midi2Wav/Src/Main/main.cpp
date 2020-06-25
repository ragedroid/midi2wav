/////////////////////////////////////////////////////////////////////
// MIDI file playback
//
// This program loads a GM soundbank (SF2) and a MIDI file (.mid)
// and produces a wave file.
//
// use: midi2wav [-gn] [-vn] [-s] infile.mid outfile.wav
// -g = GM level 1 or 2, or 0 for not GM.
// -v = master volume level
// -s = silent output
/////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <BasicSynth.h>
#include <Instruments.h>
#include <SFFile.h>
#include <SMFFile.h>
#include <GMPlayer.h>

void useage()
{
	fprintf(stderr, "MIDI File Player\n");
	fprintf(stderr, "use: midi2wav [-gn] [-vn] [-s] midifile wavefile\n");
	fprintf(stderr, "    -vn = master volume level, default 1.0\n");
	fprintf(stderr, "    -nn = maximum voices, default 32\n");
//	fprintf(stderr, "    -gn = GM level n=1 or n=2 (default 1), n=0 use bank select as is\n");
	fprintf(stderr, "    -s  = don't print info or time\n");
	exit(1);
}

int     verbose = 1;
time_t  clkTime = 0;
long    genTime = 0;
bsString midFile;
bsString sbFile;
bsString wavFile;
/* int length = 0; */

void midname(void) 
{
	const char* pmidname;
	pmidname = midFile;
    while (*pmidname) {
        pmidname++;
    }
    while (*pmidname != '/' && *pmidname != '\\') {
        pmidname--;
    }
    pmidname++;
    printf("Converting %s\n",pmidname);
}

void generatewav(void)
{
	wavFile = midFile;
	const char* pmidname;
	pmidname = midFile;
    while (*pmidname) {
        pmidname++;
    }
    while (*pmidname != '/' && *pmidname != '\\') {
        pmidname--;
    }
	bsString name;
	name = pmidname; //extract name of file
	wavFile.SetLen(wavFile.Length()-name.Length()-4);
	const char* s1 = "Wav"; //directory changed by string manipulation
	wavFile = wavFile.Append(s1); 
	name.SetLen(name.Length()-3);
	const char* s2 = "wav"; //extension replaced from mid to wav
	name = name.Append(s2); 
	wavFile = wavFile.Append(name); //file name with ext appended with path
}

void GenCallback(bsInt32 count, Opaque arg)
{
	if (verbose)
		fprintf(stderr, "Time %02ld:%02ld\r", (genTime / 60), (genTime %60));
	genTime = count;
}

int main(int argc, char *argv[])
{
	InitSynthesizer();

	int gmbank = 1;
	int maxNotes = 32;
	AmpValue vol = 2.0;

	WaveFile wvf;
	Sequencer seq;
	Mixer mix;
	InstrManager inmgr;
	SFFile sf;
	SMFFile smf;
	SMFInstrMap map[16];
	int i;
	int nchnls;
	bsInt32 chnls[16];

	int argn = 1;
	char *ap = argv[argn];
	while (argn < argc && *ap == '-')
	{
		if (ap[1] == 'v')
			vol = atof(&ap[2]);
		else if (ap[1] == 's')
			verbose = 0;
		else if (ap[1] == 'g')
			gmbank = atoi(&ap[2]);
		else if (ap[1] == 'n')
			maxNotes = atoi(&ap[2]);
		else
			useage();
		argn++;
		ap = argv[argn];
	}

//	if (argn < argc)
//		sbFile = argv[argn++];
	sbFile = "../Soundfont/GeneralUser_GS.sf2";
	if (argn < argc)
		midFile = argv[argn++]; //taking input from the terminal
	generatewav(); //wavFile generation
//	if (argn < argc)
//		wavFile = argv[argn];
	if (sbFile.Length() == 0 || midFile.Length() == 0  || wavFile.Length() == 0 )
		useage();

	SoundBank *sb = 0;
	if (SFFile::IsSF2File(sbFile))
		sb = sf.LoadSoundBank(sbFile, 0);
	
	if (!sb)
	{
		fprintf(stderr, "Failed to load soundbank %s\n", (const char *)sbFile);
		exit(1);
	}
	sb->Lock();

	if (verbose)
	{
		printf("Soundbank:\n%s\nVersion %d.%d.%d.%d\n%s\n\n", (const char*)sb->info.szName,
			sb->info.wMajorFile, sb->info.wMinorFile,
			sb->info.wMajorVer, sb->info.wMinorVer,
			(const char*)sb->info.szCopyright);
	}

	smf.GMBank(gmbank);

	if (smf.LoadFile(midFile))
	{
		fprintf(stderr, "Errror loading .mid file %s\n", (const char *)midFile);
		exit(1);
	}

	// This shows using one player config for all channels.
	// When	using the GMManager, instruments get the patch
	// directly from the MIDIChannelControl object. Envelope
	// is set from the SF2 file.

	// Add GMManager type to instrument manager
	InstrMapEntry *ime = inmgr.AddType("GMPlayer",
		GMPlayer::InstrFactory,
		GMPlayer::EventFactory,
		NULL);

	// Create the template instrument
	GMPlayer *instr = new GMPlayer;
	//instr->SetSoundFile(sb->name);
	instr->SetSoundBank(sb);
	instr->SetParam(GMPLAYER_FLAGS, (float)(GMPLAYER_LOCAL_PAN|GMPLAYER_LOCAL_VOL));

	// Add it as template to the available instrument list.
	InstrConfig *inc = inmgr.AddInstrument(0, ime, instr);
	for (i = 0; i < 16; i++)
	{
		map[i].inc = inc;
		// The bank and patch params can be used to send
		// the preset number to the instrument in the START event.
		map[i].bnkParam = GMPLAYER_BANK;
		map[i].preParam = GMPLAYER_PROG;
	}

	smf.GenerateSeq(&seq, &map[0], sb, 0xffff);
	nchnls = smf.GetChannelMap(chnls);
	if (nchnls == 0)
	{
		fprintf(stderr, "No notes found!\n");
		exit(0);
	}

	// Initialize the mixer
	mix.MasterVolume(vol, vol);
	mix.SetChannels(nchnls);
	for (i = 0; i < nchnls; i++)
	{
		mix.ChannelVolume(i, 1.0);
		mix.ChannelOn(i, chnls[i] > 0);
	}

	// run the sequencer
	wvf.OpenWaveFile(wavFile, 2);
	inmgr.Init(&mix, &wvf);
	if (verbose) midname();
	seq.SetCB(GenCallback, synthParams.isampleRate, 0);
	genTime = 0;
	time(&clkTime);
	seq.SetMaxNotes(maxNotes);
	seq.Sequence(inmgr, 0, 0);
	wvf.CloseWaveFile();

// to display time
/*	if (verbose)
	{
		long clkTimeDiff = (long) (time(0) - clkTime);
		printf("\nTime taken: %02ld:%02ld\n",
			genTime / 60, genTime % 60);
	} */ 

	sb->Unlock();
	printf("Successfully converted.\n");

	return 0;
}
