

/**
 * Common library for Pioneer CDJ protocol code (ProLink)
 *
 * N.B. this is reverse engineed from packet snooping.
 * 
 * @author teknopaul
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "cdj.h"


unsigned char CDJ_MAGIC_HEADER[10] = {0x51, 0x73, 0x70, 0x74, 0x31, 0x57, 0x6d, 0x4a, 0x4f, 0x4c};

const char*
cdj_type_to_string(int port, unsigned char type)
{
    switch (port) {
        case CDJ_DISCOVERY_PORT : {
            switch (type) {
                case 0x00 : {
                    return "CDJ_PLAYER_NUM_DISCOVERY";
                }
                case 0x02 : {
                    return "CDJ_STAGE2_DISCOVERY";
                }
                case 0x04 : {
                    return "CDJ_FINAL_DISCOVERY";
                }
                case 0x06 : {
                    return "CDJ_DEVICE_KEEP_ALIVE";
                }
                case 0x0a : {
                    return "CDJ_DISCOVERY";
                }
            }
        }
        case CDJ_BROADCAST_PORT :
        case CDJ_UPDATE_PORT :
        default : {
            switch (type) {
                case 0x00 : {
                    return "CDJ_PLAYER_NUM_DISCOVERY";
                }
                case 0x02 : {
                    return "CDJ_FADER_START_COMMAND";
                }
                case 0x03 : {
                    return "CDJ_CHANNELS_ON_AIR";
                }
                case 0x04 : {
                    return "CDJ_GOODBYE";
                }
                case 0x05 : {
                    return "CDJ_MEDIA_QRY";
                }
                case 0x06 : {
                    return "CDJ_MEDIA_RESP";
                }
                case 0x0a : {
                    return "CDJ_STATUS";
                }
                case 0x19 : {
                    return "CDJ_LOAD_TRACK_COMMAND";
                }
                case 0x1a : {
                    return "CDJ_LOAD_TRACK_ACK";
                }
                case 0x26 : {
                    return "CDJ_MASTER_HANDOFF_REQ";
                }
                case 0x27 : {
                    return "CDJ_MASTER_HANDOFF_RESP";
                }
                case 0x28 : {
                    return "CDJ_BEAT";
                }
                case 0x29 : {
                    return "CDJ_MIXER_STATUS";
                }
                case 0x2a : {
                    return "CDJ_SYNC_CONTROL";
                }
                default : return "UNKNOWN";
            }
        }
    }
}

int
cdj_header_len(int port, unsigned char type)
{
    if (port == CDJ_DISCOVERY_PORT) {
        return 12;
    }

    return 11;
}

/**
 * Convert string IP address to CDJ message format.
 */
int
cdj_ip_format(const char* ip_address, unsigned char* ip)
{
    int addr = inet_addr(ip_address);
    if (addr == INADDR_NONE) return CDJ_ERROR;
    ip[3] = (unsigned char) (addr >> 24);
    ip[2] = (unsigned char) (addr >> 16);
    ip[1] = (unsigned char) (addr >> 8);
    ip[0] = (unsigned char) (addr >> 0);
    return CDJ_OK;
}

// convert CDJ format to Linux standard
struct sockaddr_in*
cdj_ip_decode(unsigned int ip)
{
    struct sockaddr_in* ip_addr = (struct sockaddr_in*) calloc(1, sizeof(struct sockaddr_in*));
    if (ip_addr) {
        ip_addr->sin_family = AF_INET;
        ip_addr->sin_addr.s_addr = htonl(ip);
    }
    return ip_addr;
}

// convert to CDJ format
unsigned int
cdj_ip_encode(struct sockaddr_in* ip_addr)
{
    return ntohl(ip_addr->sin_addr.s_addr);
}


/**
 * validate that a received packet appears to be the correct protocol
 * packet_type - returns the packet type
 */
int
cdj_validate_header(unsigned char* data, int len, int port, int* packet_type) {

    if (len < CDJ_PACKET_TYPE_OFFSET) {
        perror("packet too short to be a ProDJLink packet");
        return CDJ_ERROR;
    }

    if (len < CDJ_MAGIC_HEADER_LENGTH || memcmp(CDJ_MAGIC_HEADER, data, CDJ_MAGIC_HEADER_LENGTH ) != 0 ) {
        perror("packet has correct header for the ProDJLink protocol.");
        return CDJ_ERROR;
    }

/* TODO validate incomming port is sane
    final Map<Byte, PacketType> portMap = PACKET_TYPE_MAP.get(port);
    if (portMap == null) {
        perror("Do not know any Pro DJ Link packets that are received on port " + port + ".");
        return CDJ_ERROR;
    }
*/
    if (packet_type) *packet_type = data[CDJ_PACKET_TYPE_OFFSET];

    return CDJ_OK;
}

/**
 * Takes a UDP packet and returns a pointer to newly malloced struct
 * that helps describe the packet, or NULL
 * @param UDP - data
 * @param packet_len - length of data
 * @param struct_len - size of real struct to create
 */
cdj_generic_packet_t*
cdj_new_generic_packet(unsigned char* packet, int packet_len, size_t struct_len)
{
    if (packet_len < CDJ_PACKET_TYPE_OFFSET + 2) return NULL;
    cdj_generic_packet_t* a_pkt = (cdj_generic_packet_t*) calloc(1, struct_len);
    if (a_pkt != NULL) {
        a_pkt->data = packet;
        a_pkt->len = packet_len;
        a_pkt->type = packet[CDJ_PACKET_TYPE_OFFSET];
    }
    return a_pkt;
}

cdj_discovery_packet_t*
cdj_new_discovery_packet(unsigned char* packet, int len, int port)
{
    if (cdj_validate_header(packet, len, port, NULL) != CDJ_OK) {
        return NULL;
    }

    cdj_discovery_packet_t* d_pkt = (cdj_discovery_packet_t*) cdj_new_generic_packet(packet, len, sizeof(cdj_discovery_packet_t));
    if (d_pkt != NULL) {
        d_pkt->player_id = cdj_discovery_player_id(d_pkt);
        d_pkt->device_type = cdj_discovery_device_type(d_pkt);

        d_pkt->ip = cdj_discovery_ip(d_pkt);
        char* mac = cdj_discovery_mac(d_pkt);
        if (mac) {
            memcpy(d_pkt->mac, mac, 6);
            free(mac);
        }

    }

    return d_pkt;
}

cdj_beat_packet_t*
cdj_new_beat_packet(unsigned char* packet, int len, int port)
{
    if (cdj_validate_header(packet, len, port, NULL) != CDJ_OK) {
        return NULL;
    }

    cdj_beat_packet_t* b_pkt = (cdj_beat_packet_t*) cdj_new_generic_packet(packet, len, sizeof(cdj_beat_packet_t));
    if (b_pkt != NULL) {
        b_pkt->player_id = cdj_beat_player_id(b_pkt);
        b_pkt->bpm = cdj_beat_calculated_bpm(b_pkt);
        b_pkt->device_type = cdj_beat_device_type(b_pkt);
        b_pkt->bar_pos = cdj_beat_bar_pos(b_pkt);
    }

    return b_pkt;
}

cdj_cdj_status_packet_t*
cdj_new_cdj_status_packet(unsigned char* packet, int len, int port) {

    if (cdj_validate_header(packet, len, port, NULL) != CDJ_OK) {
        return NULL;
    }

    cdj_cdj_status_packet_t* cs_pkt = (cdj_cdj_status_packet_t*) cdj_new_generic_packet(packet, len, sizeof(cdj_cdj_status_packet_t));
    if (cs_pkt != NULL) {
        cs_pkt->player_id = cdj_status_player_id(cs_pkt);
        cs_pkt->bpm = cdj_status_calculated_bpm(cs_pkt);
    }
    return cs_pkt;
}

cdj_mixer_status_packet_t*
cdj_new_mixer_status_packet(unsigned char* packet, int len, int port) {

    if (cdj_validate_header(packet, len, port, NULL) != CDJ_OK) {
        return NULL;
    }

    cdj_mixer_status_packet_t* ms_pkt = (cdj_mixer_status_packet_t*) cdj_new_generic_packet(packet, len, sizeof(cdj_mixer_status_packet_t));

    return ms_pkt;
}

// Message intepretation functions

// Some of these methods return 0 rather than risk index out of bounds errors, but its the callers responsibiltiy to make sure the
// data and message call is sane.  This is done so we can return unsigned char which is much more convenient.


double
cdj_pitch_to_percentage(int pitch) {
    return (pitch - 1048567) / 10485.76;
}

double
cdj_pitch_to_multiplier(int pitch) {
    return pitch / 104857600.0;
}

float
cdj_calculated_bpm(unsigned int track_bpm, unsigned int pitch)
{
    return (float) (track_bpm * cdj_pitch_to_multiplier(pitch));
}

static unsigned int
cdj_read_uint(unsigned char* data, int pos)
{
    unsigned int i = 0;
    i |= data[pos++] << 24;
    i |= data[pos++] << 16;
    i |= data[pos++] << 8;
    i |= data[pos++] << 0;
    return i;
}

static unsigned int
cdj_read_u16int(unsigned char* data, int pos)
{
    unsigned int i = 0;
    i |= data[pos++] << 8;
    i |= data[pos++] << 0;
    return i;
}

// generic
unsigned char
cdj_packet_type(unsigned char* packet, int len)
{
    if (len < CDJ_PACKET_TYPE_OFFSET + 1) return 255;
    return packet[CDJ_PACKET_TYPE_OFFSET];
}

char*
cdj_model_name(unsigned char* packet, int len, int port)
{
    if (len < 0x20) return "unknonw";
    return (char *) packet + cdj_header_len(port, cdj_packet_type(packet, len));
}

// discovery
unsigned char
cdj_discovery_player_id(cdj_discovery_packet_t* d_pkt)
{
    if (d_pkt->len < 0x25) return 0;
    return d_pkt->data[0x24];
}
unsigned char
cdj_discovery_device_type(cdj_discovery_packet_t* d_pkt)
{
    if (d_pkt->len < 0x24) return 0;
    return d_pkt->data[0x25];
}
unsigned int
cdj_discovery_ip(cdj_discovery_packet_t* d_pkt)
{
    switch (cdj_packet_type(d_pkt->data, d_pkt->len)) {
        case CDJ_DISCOVERY:
        case CDJ_PLAYER_NUM_DISCOVERY: {
            return 0;
        }
        case CDJ_STAGE2_DISCOVERY: {
            if (d_pkt->len < 0x27) return 0;
            return cdj_read_uint(d_pkt->data, 0x24);
        }
        case CDJ_KEEP_ALIVE: {
            if (d_pkt->len < 0x2f) return 0;
            return cdj_read_uint(d_pkt->data, 0x2c);
        }
    }
    return 0;
}

char*
cdj_discovery_mac(cdj_discovery_packet_t* d_pkt)
{
    char* mac = (char*) calloc(1, 6);
    if (mac == NULL) return NULL;

    switch (cdj_packet_type(d_pkt->data, d_pkt->len)) {
        case CDJ_KEEP_ALIVE:
        case CDJ_PLAYER_NUM_DISCOVERY: {
            if (d_pkt->len < 0x2b) return 0;
            mac[0] = d_pkt->data[0x26];
            mac[1] = d_pkt->data[0x27];
            mac[2] = d_pkt->data[0x28];
            mac[3] = d_pkt->data[0x29];
            mac[4] = d_pkt->data[0x2a];
            mac[5] = d_pkt->data[0x2b];
        }
        case CDJ_STAGE2_DISCOVERY: {
            if (d_pkt->len < 0x2d) return 0;
            mac[0] = d_pkt->data[0x28];
            mac[1] = d_pkt->data[0x29];
            mac[2] = d_pkt->data[0x2a];
            mac[3] = d_pkt->data[0x2b];
            mac[4] = d_pkt->data[0x2c];
            mac[5] = d_pkt->data[0x2d];
        }
    }
    return mac;
}

// cdj status, need to be careful here because some different playeer send different mounts of information

unsigned char
cdj_status_player_id(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x1f) return 0;
    return cs_pkt->data[0x20];
}

// should be 1 or zero, 1 means busy, not necessarilyplaying audio, can be loading a track
unsigned char
cdj_status_active(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x26) return 0;
    return cs_pkt->data[0x27];
}
/**
 * player_id this CDJ is playing from. If zero no track loaded
 */
unsigned char
cdj_status_playing_from(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x27) return 0;
    return cs_pkt->data[0x28];
}
/**
 * 00=no track, 01=CD, 02=SD, 03=USB, 04=rekordbox
 */
unsigned char
cdj_status_playing_from_slot(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x28) return 0;
    return cs_pkt->data[0x29];
}
/**
 * 00=no track, 01=have beat grid, 02=no beat grid, 05=loaded from CD
 */
unsigned char
cdj_status_track_type(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x29) return 0;
    return cs_pkt->data[0x2a];
}
/**
 * recordbox track id (labeled `recordbox` in pdf), or CD track number, number used to request track meta data
 */
unsigned int
cdj_status_track_id(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x2e) return 0;
    return cdj_read_uint(cs_pkt->data, 0x2c);
}
/**
 * Load a uint alto I think the devices are actuall limited to 4000 tracks per playlist.
 * Potentialy can be used to auto-play a whole playlist?
 */
unsigned int
cdj_status_track_number(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x32) return 0;
    return cdj_read_uint(cs_pkt->data, 0x30);
}
/**
 * 00 no CD, 1e audio CD loaded, 11 MP3/data disk
 */
unsigned int
cdj_status_cd_data_type(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x39) return 0;
    return cdj_read_uint(cs_pkt->data, 0x37);
}
/*
Value   Meaning
00      No track is loaded
02      A track is in the process of loading
03      Player is playing normally
04      Player is playing a loop
05      Player is paused anywhere other than the cue point
06      Player is paused at the cue point
07      Cue Play is in progress (playback while the cue button is held down)
08      Cue scratch is in progress
09      Player is searching forwards or backwards
0e      Audio CD has spun down due to lack of use
11      Player reached the end of the track and stopped
 */
unsigned int
cdj_status_play_mode(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x7a) return 0;
    return cs_pkt->data[0x7b];
}

unsigned int
cdj_status_sync_counter(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x84) return 0;
    return cdj_read_u16int(cs_pkt->data, 0x84);
}
/**
 * return a bit mask, or play state, e.g. flag & with CDJ_PLAY_STATE_*
 */
unsigned char
cdj_status_sync_flags(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x88) return 0;
    return cs_pkt->data[0x89];
}
/**
 * P2 play state indicator different per CDJ type
 */
unsigned int
cdj_status_sync_state(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x8d) return 0;
    return cdj_read_uint(cs_pkt->data, 0x8b);
}
/**
 * @returns  CDJ_MASTER_STATE_* 00 not master, 01 tempo master, 02 master but no beat grid
 */
unsigned char
cdj_status_master_state(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x9d) return 0;
    return cs_pkt->data[0x9e];
}
/**
 * player id of a new master being negotiated
 */
unsigned char
cdj_status_new_master(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x9e) return 0;
    return cs_pkt->data[0x9f];
}

/**
 * returns 1 indexed position of the beat in the bar
 */
unsigned char
cdj_status_bar_pos(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0xa5) return 0;
    return cs_pkt->data[0xa6];
}


unsigned int
cdj_status_bpm(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x92) return 0;
    return cdj_read_u16int(cs_pkt->data, 0x92);
}
unsigned int
cdj_status_pitch(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0x8e) return 0;
    return cdj_read_uint(cs_pkt->data, 0x8c);
}
float
cdj_status_calculated_bpm(cdj_cdj_status_packet_t* cs_pkt) {
    return cdj_calculated_bpm(cdj_status_bpm(cs_pkt), cdj_status_pitch(cs_pkt));
}
unsigned int
cdj_status_pitch_per_fader(cdj_cdj_status_packet_t* cs_pkt)
{
    if (cs_pkt->len < 0xc2) return 0;
    return cdj_read_uint(cs_pkt->data, 0xc0);
}


// beat

unsigned int
cdj_beat_bpm(cdj_beat_packet_t* b_pkt)
{
    if (b_pkt->len < 0x5a) return 0;
    return cdj_read_u16int(b_pkt->data, 0x5a);
}
unsigned int
cdj_beat_pitch(cdj_beat_packet_t* b_pkt)
{
    if (b_pkt->len < 0x56) return 0;
    return cdj_read_uint(b_pkt->data, 0x54);
}
float
cdj_beat_calculated_bpm(cdj_beat_packet_t* b_pkt) 
{
    return cdj_calculated_bpm(cdj_beat_bpm(b_pkt), cdj_beat_pitch(b_pkt));
}
unsigned char
cdj_beat_player_id(cdj_beat_packet_t* b_pkt)
{
    if (b_pkt->len < 0x20) return 0;
    return b_pkt->data[0x21];
}
unsigned char
cdj_beat_device_type(cdj_beat_packet_t* b_pkt)
{
    if (b_pkt->len < 0x1e) return 0;
    return b_pkt->data[0x1f];
}
unsigned char
cdj_beat_bar_pos(cdj_beat_packet_t* b_pkt)
{
    if (b_pkt->len < 0x5b) return 0;
    return b_pkt->data[0x5c];
}


// TODO effective cdj_status_bpm

// Message creation functions, based on CDJ_Protocol_Analysis.pdf
// hence lots of magic numbers we dont really know what mean.

/**
 * Set the 11 byte header
 */
static void
cdj_set_header(unsigned char* packet_pt, unsigned char type)
{
    memcpy(packet_pt, CDJ_MAGIC_NUMBER, 10);
    packet_pt[10] = type;
}

static void
cdj_set_device_name(unsigned char* packet_pt, unsigned char model)
{
    memcpy(packet_pt, "CDJ-1000\0\0\0\0\0\0\0\0\0\0\0\0", CDJ_DEVICE_NAME_LENGTH);
    packet_pt[0] = model;
}

// CDJ_Protocol_Analysis.pdf Figure 6
unsigned char*
cdj_create_initial_discovery_packet(int* length, unsigned char model, char device_type)
{
    *length = 37;
    unsigned char* packet = (unsigned char*) calloc(1, *length);
    if (packet) {
        cdj_set_header(packet, CDJ_DISCOVERY);
        cdj_set_device_name(packet + cdj_header_len(50000, CDJ_DISCOVERY), model);
        packet[32] = 0x01;
        packet[33] = 0x02;
        packet[34] = 0x00;
        packet[35] = *length;
        packet[36] = device_type; // CDJ (mixer is 0x02)
    }
    return packet;
}

// CDJ_Protocol_Analysis.pdf Figure 7
unsigned char*
cdj_create_stage1_discovery_packet(int* length, unsigned char model, char device_type, unsigned char *mac, unsigned char n)
{
    *length = 44;
    unsigned char* packet = (unsigned char*) calloc(1, *length);
    if (packet) {
        cdj_set_header(packet, CDJ_PLAYER_NUM_DISCOVERY);
        cdj_set_device_name(packet + cdj_header_len(50000, CDJ_DISCOVERY), model);
        packet[32] = 0x01;
        packet[33] = 0x02;
        packet[34] = 0x00;
        packet[35] = *length;
        packet[36] = n;
        packet[37] = device_type; // CDJ (mixer is 0x02)
        memcpy(packet + 38, mac, 6);
    }
    return packet;
}

void 
cdj_inc_stage1_discovery_packet(unsigned char* packet)
{
    packet[36]++;
}


// NOT SENT ??

unsigned char*
cdj_create_stage2_discovery_packet(int* length, unsigned char model, char device_type, unsigned char* ip, unsigned char* mac, unsigned char player_id, unsigned char n)
{
    *length = 50;
    unsigned char* packet = (unsigned char*) calloc(1, *length);
    if (packet) {
        cdj_set_header(packet, CDJ_STAGE2_DISCOVERY);
        cdj_set_device_name(packet + cdj_header_len(50000, CDJ_STAGE2_DISCOVERY), model);
        packet[32] = 0x01;
        packet[33] = 0x02;
        packet[34] = 0x00;
        packet[35] = *length;
        memcpy(packet + 36, ip, 4);
        memcpy(packet + 40, mac, 6);
        packet[46] = player_id;
        packet[47] = n;
        packet[48] = device_type; // CDJ (mixer is 0x02)
        packet[49] = 0x01; // unknown
    }
    return packet;
}

void 
cdj_inc_stage2_discovery_packet(unsigned char* packet)
{
    packet[47]++;
}

unsigned char*
cdj_create_final_discovery_packet(int* length, unsigned char model, char device_type, unsigned char player_id, unsigned char n)
{
    *length = 38;
    unsigned char* packet = (unsigned char*) calloc(1, *length);
    if (packet) {
        cdj_set_header(packet, CDJ_FINAL_DISCOVERY);
        cdj_set_device_name(packet + cdj_header_len(50000, CDJ_DISCOVERY), model);
        packet[32] = 0x01;
        packet[33] = 0x02;
        packet[34] = 0x00;
        packet[35] = *length;
        packet[36] = player_id;
        packet[37] = n;
    }
    return packet;
}

void 
cdj_inc_final_discovery_packet(unsigned char* packet)
{
    packet[47]++;
}

unsigned char*
cdj_create_keepalive_packet(int* length, unsigned char model, char device_type, unsigned char* ip, unsigned char* mac, unsigned char player_id)
{
    *length = 54;
    unsigned char* packet = (unsigned char*) calloc(1, *length);
    if (packet) {
        cdj_set_header(packet, CDJ_KEEP_ALIVE);
        cdj_set_device_name(packet + cdj_header_len(50000, CDJ_DISCOVERY), model);
        packet[32] = 0x01;
        packet[33] = 0x02;
        packet[34] = 0x00;
        packet[35] = *length;
        packet[36] = player_id;
        packet[0x25] = 0x02;
        memcpy(packet + 38, mac, 6);
        memcpy(packet + 44, ip, 4);
        packet[48] = 0x01;
        packet[49] = 0x00;
        packet[50] = 0x00;
        packet[51] = 0x00;
        packet[52] = device_type; // CDJ (mixer is 0x02)
        packet[53] = 0x00;
    }
    return packet;
}

static int
cdj_beat_millis(float bpm)
{
    return (int) (60000.0 / bpm);
}

static void
cdj_set_int(unsigned char* packet_pt, unsigned int i)
{
    *packet_pt++ = (unsigned char) (i  >> 24);
    *packet_pt++ = (unsigned char) (i  >> 16);
    *packet_pt++ = (unsigned char) (i  >> 8);
    *packet_pt++ = (unsigned char) (i  >> 0);
}
static void
cdj_set_u16int(unsigned char* packet_pt, unsigned int i)
{
    *packet_pt++ = (unsigned char) (i  >> 8);
    *packet_pt++ = (unsigned char) (i  >> 0);
}
/**
 * Figure 9 beat packets,
 * this code presumes constant bpm and/or that CDJs can handle the fact that between beat messages BPM may change.
 * a lot of redundant info for house music, I dont see when the client armed with beat time and BPM cant do this calculation themselves, or why the need it?
 *
 * N.B> Pioneer send the tracks base BPM and then the pitch adjust, instead of sending the actual BPM (TODO presumably tempo for master tempo mde)
 * Bu tthe BPM is sent every message so one wonders why?  Maybe to enable key matching?
 *
 * @param bar_index - 0 ,3, where in the bar we are, presumes 4/4
 */ 
unsigned char*
cdj_create_beat_packet(int* length, unsigned char model, char device_type, unsigned char player_id, float bpm, unsigned char bar_index)
{
    *length = 0x60;
    int beat_offset;
    unsigned int beat_pos;
    unsigned int bar_pos;
    unsigned int millis_in_a_beat = cdj_beat_millis(bpm);

    unsigned char* packet = (unsigned char*) calloc(1, *length);
    if (packet) {
        cdj_set_header(packet, CDJ_BEAT);
        cdj_set_device_name(packet + cdj_header_len(50001, CDJ_BEAT), model);
        packet[0x1f] = 0x01;       // could this be device type CDJ, seems to be always 1 even for XDJ
        packet[0x20] = 0x00;
        packet[0x21] = player_id;  // device number or player number?
        packet[0x22] = 0x00;
        packet[0x23] = 0x3c;       // what is this? (broken length value?)

        beat_offset = 0x24;
        
        // nextBeat
        beat_pos = millis_in_a_beat;
        cdj_set_int(packet + beat_offset, beat_pos); beat_offset += 4;
        // 2ndBeat
        beat_pos += millis_in_a_beat;
        cdj_set_int(packet + beat_offset, beat_pos); beat_offset += 4;
        // nextBar
        bar_pos = (4 - bar_index) * millis_in_a_beat;
        cdj_set_int(packet + beat_offset, bar_pos); beat_offset += 4;
        // 4thBeat
        beat_pos += 2 * millis_in_a_beat;
        cdj_set_int(packet + beat_offset, beat_pos); beat_offset += 4;
        // 2ndBar
        bar_pos = (8 - bar_index) * millis_in_a_beat;
        cdj_set_int(packet + beat_offset, bar_pos); beat_offset += 4;
        //8thBeat
        beat_pos += 4 * millis_in_a_beat;
        cdj_set_int(packet + beat_offset, beat_pos); beat_offset += 4;

        memset(packet + 0x3c, 0xff, 24);

        // TODO pitch, midi does not pitch adjust
        cdj_set_int(packet + 0x54, 0x00100000);

        // bpm (protocol hardcodes to 2 decimal places, but seems to reserve space)
        int bpm_i = (int) (bpm * 100);
        cdj_set_int(packet + 0x58, bpm_i);

        // b, bar index
        packet[0x5c] = bar_index + 1;

        packet[0x5f] = player_id;
    }
    return packet;
}


unsigned char*
cdj_create_status_packet(int* length, unsigned char model, unsigned char player_id, float bpm, unsigned char bar_index, char master, unsigned int n)
{
    *length = 0xd4;
    unsigned char* packet = (unsigned char*) calloc(1, *length);
    if (packet) {
        cdj_set_header(packet, CDJ_STATUS);
        cdj_set_device_name(packet + cdj_header_len(50001, 0), model);
        packet[0x1f] = 0x01;
        packet[0x20] = 0x03;
        packet[0x21] = player_id;
        packet[0x23] = 0xb0;
        packet[0x24] = player_id;
        packet[0x25] = 0x00;
        packet[0x26] = 0x01;

        packet[0x27] = 0x01;       // A active
        packet[0x28] = player_id;  // Dr trackloaded from myself
        packet[0x29] = 0x03;       // Sr track loaded from USB
        packet[0x2a] = 0x01;       // Tr track supports beat grid (we thus have to bradcast beat frames)


        packet[0x3a] = 0xa0;

        packet[0x37] = 0x05;
        packet[0x47] = 0x08;

        // track id
        cdj_set_int(packet + 0x2c, 0x01); // static 1 for the moment since we don yet support media queries
        cdj_set_int(packet + 0x30, 0x01); // track number

        // play mode
        packet[0x7b] = 0x03;       // normal

        // firmware version  31 2e 30 35  "1.05" XDJ does not seem to care
        packet[0x7c] = 0x31;  // 1
        packet[0x7d] = 0x2e;  // .
        packet[0x7e] = 0x30;  // 0
        packet[0x7f] = 0x35;  // 5

        // sync counter
        cdj_set_int(packet + 0x84, 0x01);  // TODO should come from highes sync_counter in the backline

        packet[0x89] = CDJ_PLAY_STATE_BASE | CDJ_PLAY_STATE_PLAY; // TODO nexus flags play & master & sync & onair
        if (master) {
            packet[0x89] |= CDJ_PLAY_STATE_MASTER;
        }
        packet[0x8a] = 0xff;

        // pitch
        cdj_set_int(packet + 0x8c, CDJ_PITCH_NORMAL);
        cdj_set_int(packet + 0x98, CDJ_PITCH_NORMAL);
        cdj_set_int(packet + 0xc0, CDJ_PITCH_NORMAL);
        cdj_set_int(packet + 0xc4, CDJ_PITCH_NORMAL);

        // bpm
        // Mv 
        cdj_set_u16int(packet + 0x90, 0x8000); // TODO Mv for master handoffs  backline->master_new
        // bpm x 100  as an int
        cdj_set_u16int(packet + 0x92, (int) (bpm * 100.0));

        packet[0x9e] = master ? : 0x01 | 0xff;  // I am the master
        packet[0x9f] = 0xff;  // master handoff, new_master goes here if we get sent a master_req

        cdj_set_int(packet + 0xc4, CDJ_PITCH_NORMAL);
        // magic / unknown
        cdj_set_int(packet + 0x94, 0x7fffffff);

        cdj_set_int(packet + 0xc8, n);
        packet[0xcc] = 0x0f;  // I am nexus

    }
    return packet;
}


unsigned char*
cdj_create_master_request_packet(int* length, unsigned char model, unsigned char player_id)
{
    *length = 0x27;
    unsigned char* packet = (unsigned char*) calloc(1, *length);
    if (packet) {
        cdj_set_header(packet, CDJ_MASTER_REQ);
        cdj_set_device_name(packet + cdj_header_len(50001, 0), model);
        packet[0x1f] = 0x01;
        packet[0x21] = player_id;
        packet[0x23] = 0x04;
        packet[0x27] = player_id;
    }
    return packet;
}

/**
 * Accept a take over request, DM to requester
 */
unsigned char*
cdj_create_master_response_packet(int* length, unsigned char model, unsigned char player_id)
{
    *length = 0x2b;
    unsigned char* packet = (unsigned char*) calloc(1, *length);
    if (packet) {
        cdj_set_header(packet, CDJ_MASTER_REQ);
        cdj_set_device_name(packet + cdj_header_len(50001, 0), model);
        packet[0x1f] = 0x01;
        packet[0x21] = player_id;
        packet[0x23] = 0x08;
        packet[0x27] = player_id;
        packet[0x2b] = 0x01;
    }
    return packet;
}

unsigned char*
cdj_create_sync_control_packet(int* length, unsigned char model, unsigned char player_id, int on_off)
{
    *length = 0x2b;
    unsigned char* packet = (unsigned char*) calloc(1, *length);
    if (packet) {
        cdj_set_header(packet, CDJ_SYNC_CONTROL);
        cdj_set_device_name(packet + cdj_header_len(50001, 0), model);
        packet[0x1f] = 0x01;
        packet[0x21] = player_id;
        packet[0x23] = 0x08;
        packet[0x27] = player_id;
        packet[0x2b] = on_off ? 0x10 : 0x20;
    }
    return packet;
}

// DEBUG

void
cdj_print_packet(unsigned char* packet, int length, int port)
{
    int i;
    char* str = (char*) calloc(1, (length * 4) + 5 + length / 32);
    if (str == NULL) return;

    int type = packet[CDJ_PACKET_TYPE_OFFSET];
    int hdr_len = cdj_header_len(port, type);

    char* s = str;

    for (i = 0; i < length; i++) {
        //if (i == hdr_len ) *s++ = '\n';
        if (i > 0 && (i % 16 == 0)) *s++ = '\n';
        if (i < 10 || (i >= hdr_len && i < hdr_len + CDJ_DEVICE_NAME_LENGTH) ) {
            s += snprintf(s, 4, "%c_ ", packet[i] ? packet[i] : '_');
        }
        else s += snprintf(s, 4, "%02x ", packet[i]);
    }

    printf("packet::%03x %i\n'%s' type=%02x:%s\n%s\n", length, length, packet + hdr_len, packet[10], cdj_type_to_string(port, packet[10]), str);
    free(str);
}
