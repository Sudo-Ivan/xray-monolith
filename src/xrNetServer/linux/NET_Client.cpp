// Linux client stub: no-network implementation.
// INetQueue and client bookkeeping are real; transport methods are
// no-ops until a real socket backend is written.
#include "stdafx.h"
#include "NET_Client.h"

INetQueue::INetQueue()
{
	unused.reserve(128);
	for (int i = 0; i < 16; i++)
		unused.push_back(xr_new<NET_Packet>());
}

INetQueue::~INetQueue()
{
	cs.Enter();
	for (u32 it = 0; it < unused.size(); it++) xr_delete(unused[it]);
	for (u32 it = 0; it < ready.size(); it++) xr_delete(ready[it]);
	cs.Leave();
}

NET_Packet* INetQueue::Create()
{
	NET_Packet* P = 0;
	if (unused.empty())
	{
		ready.push_back(xr_new<NET_Packet>());
		P = ready.back();
	}
	else
	{
		ready.push_back(unused.back());
		unused.pop_back();
		P = ready.back();
	}
	return P;
}

NET_Packet* INetQueue::Create(const NET_Packet& _other)
{
	NET_Packet* P = 0;
	cs.Enter();
	if (unused.empty())
	{
		ready.push_back(xr_new<NET_Packet>());
		P = ready.back();
	}
	else
	{
		ready.push_back(unused.back());
		unused.pop_back();
		P = ready.back();
	}
	CopyMemory(P, &_other, sizeof(NET_Packet));
	cs.Leave();
	return P;
}

NET_Packet* INetQueue::Retreive()
{
	NET_Packet* P = 0;
	if (!ready.empty()) P = ready.front();
	else
	{
		u32 size = unused.size();
		if (size > 32)
		{
			xr_delete(unused.back());
			unused.pop_back();
		}
	}
	return P;
}

void INetQueue::Release()
{
	VERIFY(!ready.empty());
	u32 size = unused.size();
	ready.front()->B.count = 0;
	if (size > 32)
	{
		xr_delete(ready.front());
	}
	else
		unused.push_back(ready.front());
	ready.pop_front();
}

const u32 syncQueueSize = 512;
const int syncSamples = 256;

class XRNETSERVER_API syncQueue
{
	u32 table [syncQueueSize];
	u32 write;
	u32 count;
public:
	syncQueue() { clear(); }

	IC void push(u32 value)
	{
		table[write++] = value;
		if (write == syncQueueSize) write = 0;
		if (count <= syncQueueSize) count++;
	}

	IC u32* begin() { return table; }
	IC u32* end() { return table + count; }
	IC u32 size() { return count; }
	IC void clear()
	{
		write = 0;
		count = 0;
	}
} net_DeltaArray;

//-------
XRNETSERVER_API Flags32 psNET_Flags = {0};
XRNETSERVER_API int psNET_ClientUpdate = 30; // FPS
XRNETSERVER_API int psNET_ClientPending = 2;
XRNETSERVER_API char psNET_Name[32] = "Player";
XRNETSERVER_API BOOL psNET_direct_connect = FALSE;

//------------------------------------------------------------------------------

void IPureClient::_SendTo_LL(const void* data, u32 size, u32 flags, u32 timeout)
{
	SendTo_LL(const_cast<void*>(data), size, flags, timeout);
}

void IPureClient::_Recieve(const void* data, u32 data_size, u32 /*param*/)
{
	NET_Packet* packet = net_Queue.Create();
	packet->construct(data, data_size);
	OnMessage(packet->B.data, packet->B.count);
	net_Queue.Release();
}

IPureClient::IPureClient(CTimer* timer): net_Statistic(timer)
{
	NET = NULL;
	net_Address_server = NULL;
	net_Address_device = NULL;
	device_timer = timer;
	net_TimeDelta_User = 0;
	net_Time_LastUpdate = 0;
	net_TimeDelta = 0;
	net_TimeDelta_Calculated = 0;
	net_Connected = EnmConnectionFails;
	net_Syncronised = FALSE;
	net_Disconnected = TRUE;
}

IPureClient::~IPureClient()
{
	psNET_direct_connect = FALSE;
}

BOOL IPureClient::Connect(LPCSTR options)
{
	// No network transport on Linux yet.
	net_Disconnected = TRUE;
	net_Connected = EnmConnectionFails;
	return FALSE;
}

void IPureClient::Disconnect()
{
	if (net_Disconnected)
		return;
	net_Connected = EnmConnectionFails;
	net_Syncronised = FALSE;
	net_Disconnected = TRUE;
}

HRESULT IPureClient::net_Handler(u32 dwMessageType, PVOID pMessage)
{
	return 0;
}

void IPureClient::net_Syncronize()
{
}

void IPureClient::Sync_Thread()
{
}

void IPureClient::Sync_Average()
{
	u32 Delta = 0;
	u32 Size = net_DeltaArray.size();
	if (0 != Size)
	{
		for (u32* I = net_DeltaArray.begin(); I != net_DeltaArray.end(); I++)
			Delta += *I;
		Delta /= Size;
	}
	net_TimeDelta_Calculated = Delta;
}

void IPureClient::OnMessage(void* data, u32 size)
{
	NET_Packet P;
	P.construct(data, size);
}

void IPureClient::timeServer_Correct(u32 sv_time, u32 cl_time)
{
	u32 ping = net_Statistic.getPing();
	u32 delta = sv_time + ping / 2 - cl_time;
	net_DeltaArray.push(delta);
	Sync_Average();
}

void IPureClient::SendTo_LL(void* data, u32 size, u32 dwFlags, u32 dwTimeout)
{
	if (net_Disconnected)
		return;
	net_Statistic.dwBytesSended += size;
}

void IPureClient::Send(NET_Packet& packet, u32 dwFlags, u32 dwTimeout)
{
	MultipacketSender::SendPacket(packet.B.data, packet.B.count, dwFlags, dwTimeout);
}

void IPureClient::Flush_Send_Buffer()
{
	MultipacketSender::FlushSendBuffer(0);
}

BOOL IPureClient::net_HasBandwidth()
{
	return TRUE;
}

void IPureClient::ClearStatistic()
{
	net_Statistic.Clear();
}

void IPureClient::UpdateStatistic()
{
}

bool IPureClient::GetServerAddress(ip_address& pAddress, DWORD* pPort)
{
	return false;
}

BOOL IPureClient::net_IsSyncronised()
{
	return net_Syncronised;
}
