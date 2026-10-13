// Minimal DirectPlay8 declarations for the Linux build.
// Networking transport is not ported yet; these types satisfy the
// xrNetServer headers. Interface pointers are opaque because the
// DirectPlay implementation files are not compiled.
#pragma once

#include <stdint.h>
#include <objbase.h>
#include <string.h>


typedef unsigned long DPNID;
typedef unsigned long DPNHANDLE;
typedef unsigned long DPNHANDLE_ASYNCTASK;

struct IDirectPlay8Client;
struct IDirectPlay8Server;
struct IDirectPlay8Peer;
struct IDirectPlay8Address;
struct IDirectPlay8ThreadPool;
struct IDirectPlay8NATResolver;

struct DPN_APPLICATION_DESC
{
    unsigned long dwSize;
    unsigned long dwFlags;
    GUID guidInstance;
    GUID guidApplication;
    unsigned long dwMaxPlayers;
    unsigned long dwCurrentPlayers;
    wchar_t* pwszSessionName;
    wchar_t* pwszPassword;
    void* pvReservedData;
    unsigned long dwReservedDataSize;
    void* pvApplicationReservedData;
    unsigned long dwApplicationReservedDataSize;
};

struct DPN_PLAYER_INFO
{
    unsigned long dwSize;
    unsigned long dwInfoFlags;
    wchar_t* pwszName;
    void* pvData;
    unsigned long dwDataSize;
    unsigned long dwPlayerFlags;
};

struct DPN_BUFFER_DESC
{
    unsigned long dwSize;
    unsigned char* pBufferData;
    unsigned long dwBufferSize;
};

struct DPN_CONNECTION_INFO
{
    unsigned long dwSize;
    unsigned long dwRoundTripLatencyMS;
    unsigned long dwThroughputBPS;
    unsigned long dwPeakThroughputBPS;
    unsigned long dwBytesSentGuaranteed;
    unsigned long dwPacketsSentGuaranteed;
    unsigned long dwBytesSentNonGuaranteed;
    unsigned long dwPacketsSentNonGuaranteed;
    unsigned long dwBytesRetried;
    unsigned long dwPacketsRetried;
    unsigned long dwBytesDropped;
    unsigned long dwPacketsDropped;
    unsigned long dwMessagesTransmittedGuaranteed;
    unsigned long dwMessagesTimedOutGuaranteed;
    unsigned long dwMessagesTransmittedNonGuaranteed;
    unsigned long dwMessagesTimedOutNonGuaranteed;
    unsigned long dwBytesReceivedGuaranteed;
    unsigned long dwPacketsReceivedGuaranteed;
    unsigned long dwBytesReceivedNonGuaranteed;
    unsigned long dwPacketsReceivedNonGuaranteed;
    unsigned long dwMessagesReceived;
};

// Send flags
#define DPNSEND_GUARANTEED      0x00000001
#define DPNSEND_PRIORITY_HIGH   0x00000002
#define DPNSEND_PRIORITY_LOW    0x00000004
#define DPNSEND_NONSEQUENTIAL   0x00000008
#define DPNSEND_SEQUENTIAL      0x00000010
#define DPNSEND_NOCOMPLETE      0x00000020
#define DPNSEND_NOLOOPBACK      0x00000040
#define DPNSEND_IMMEDIATELLY    0x00000080
#define DPNSEND_COALESCE        0x00000100
#define DPNSEND_NOSYNCHRONIZE   0x00000200
#define DPNSEND_COMPLETE_ON_PROCESS 0x00000400
#define DPNSEND_UNGUARANTEED    0x00000800
#define DPNSEND_THROTTLE_SERVER 0x00001000
#define DPNSEND_NOCOPY          0x00002000
#define DPNSEND_INCOMPLETE      0x00004000
#define DPNSEND_ASYNC           0x00008000
#define DPNSEND_SEND_GUARANTEED 0x00010000

// Message ids
#define DPN_MSGID_ENUM_HOSTS_QUERY         0x0001
#define DPN_MSGID_ENUM_HOSTS_RESPONSE      0x0002
#define DPN_MSGID_INDICATED_CONNECT        0x0004
#define DPN_MSGID_INDICATE_CONNECT         DPN_MSGID_INDICATED_CONNECT
#define DPN_MSGID_CONNECT_COMPLETE         0x0008
#define DPN_MSGID_INDICATED_CONNECT_ABORTED 0x0010
#define DPN_MSGID_SERVER_INFO              0x0020
#define DPN_MSGID_RECEIVE                  0x0040
#define DPN_MSGID_SEND_COMPLETE            0x0080
#define DPN_MSGID_CREATE_PLAYER            0x0100
#define DPN_MSGID_DESTROY_PLAYER           0x0200
#define DPN_MSGID_PEER_INFO                0x0400
#define DPN_MSGID_HOST_MIGRATE             0x0800
#define DPN_MSGID_TERMINATE_SESSION        0x1000
#define DPN_MSGID_RETURN_BUFFER            0x2000
#define DPN_MSGID_ADD_PLAYER_TO_GROUP      0x4000
#define DPN_MSGID_REMOVE_PLAYER_FROM_GROUP 0x8000
#define DPN_MSGID_GROUP_INFO               0x10000
#define DPN_MSGID_CLIENT_INFO              0x20000
#define DPN_MSGID_NAT_ASSIST               0x40000
#define DPN_MSGID_NAT_RESOLVER_INFO        0x80000
#define DPN_MSGID_LISTEN_INFO              0x100000
#define DPN_MSGID_INDICATED_DISCONNECT     0x200000

// Aliases used by the engine
#define DPNMSG_RECEIVE                DPN_MSGID_RECEIVE
#define DPNMSG_CREATE_PLAYER          DPN_MSGID_CREATE_PLAYER
#define DPNMSG_DESTROY_PLAYER         DPN_MSGID_DESTROY_PLAYER
#define DPNMSG_INDICATE_CONNECT       DPN_MSGID_INDICATE_CONNECT
#define DPNMSG_TERMINATE_SESSION      DPN_MSGID_TERMINATE_SESSION
#define DPNMSG_HOST_MIGRATE           DPN_MSGID_HOST_MIGRATE
#define DPNMSG_SEND_COMPLETE          DPN_MSGID_SEND_COMPLETE

// Session flags
#define DPNSESSION_REQUIREPASSWORD    0x00000002
#define DPNSESSION_ALLOWMIGRATION     0x00000004
#define DPNSESSION_NODATAPATH         0x00000008

// Address component keys
#define DPNA_DATATYPE_STRING          0
#define DPNA_DATATYPE_DWORD           1
#define DPNA_DATATYPE_GUID            2
#define DPNA_DATATYPE_BINARY          3
#define DPNA_DATATYPE_STRING_ANSI     4
#define DPNA_KEY_HOSTNAME             0
#define DPNA_KEY_PORT                 1
#define DPNA_KEY_SESSION_NAME         2
#define DPNA_KEY_DEVICE               3

// HRESULT-ish codes used in comparisons
#define DPN_OK                        0
#define DPNERR_GENERIC                0x80004005
#define DPNERR_INVALIDPARAM           0x80070057
#define DPNERR_SESSIONFULL            0x80040105
#define DPNERR_INVALIDPASSWORD        0x80040104
#define DPNERR_HOSTREJECTEDCONNECTION 0x80040102

#define DPNID_ALL_PLAYERS_GROUP 0
#define DPNID_SERVER_PLAYER     1
