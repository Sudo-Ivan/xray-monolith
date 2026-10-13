// Linux server stub: no-network implementation.
// Interface methods are no-ops until a real socket backend is written.
#include "stdafx.h"
#include "NET_Server.h"

XRNETSERVER_API int psNET_ServerUpdate = 30; // FPS
XRNETSERVER_API int psNET_ServerPending = 3;
XRNETSERVER_API int psNET_Port = 5445;
XRNETSERVER_API ClientID BroadcastCID(0xffffffff);

IPureServer::IPureServer(CTimer* timer, BOOL Dedicated)
{
	stats.clear();
	device_timer = timer;
	m_bDedicated = Dedicated;
	SV_Client = NULL;
}

IPureServer::~IPureServer()
{
}

HRESULT IPureServer::net_Handler(u32 dwMessageType, PVOID pMessage)
{
	return 0;
}

IPureServer::EConnect IPureServer::Connect(LPCSTR session_name, GameDescriptionData& game_descr)
{
	return ErrConnect;
}

void IPureServer::Disconnect()
{
}

void IPureServer::client_link_aborted(ClientID ID)
{
}

void IPureServer::client_link_aborted(IClient* C)
{
}

IClient* IPureServer::ID_to_client(ClientID ID, bool ScanAll)
{
	return NULL;
}

bool IPureServer::GetClientAddress(IDirectPlay8Address* pClientAddress, ip_address& Address, DWORD* pPort)
{
	return false;
}

IBannedClient* IPureServer::GetBannedClient(const ip_address& Address)
{
	return NULL;
}

void IPureServer::BannedList_Save()
{
}

void IPureServer::BannedList_Load()
{
}

void IPureServer::IpList_Load()
{
}

void IPureServer::IpList_Unload()
{
}

LPCSTR IPureServer::GetBannedListName()
{
	return "";
}

void IPureServer::UpdateBannedList()
{
}

void IPureServer::SendTo_LL(ClientID ID, void* data, u32 size, u32 dwFlags, u32 dwTimeout)
{
}

void IPureServer::SendTo_Buf(ClientID ID, void* data, u32 size, u32 dwFlags, u32 dwTimeout)
{
}

void IPureServer::Flush_Clients_Buffers()
{
}

void IPureServer::SendTo(ClientID ID, NET_Packet& P, u32 dwFlags, u32 dwTimeout)
{
}

void IPureServer::SendBroadcast_LL(ClientID exclude, void* data, u32 size, u32 dwFlags)
{
}

void IPureServer::SendBroadcast(ClientID exclude, NET_Packet& P, u32 dwFlags)
{
}

void IPureServer::ClearStatistic()
{
	stats.clear();
}

void IPureServer::UpdateClientStatistic(IClient* C)
{
}

u32 IPureServer::OnMessage(NET_Packet& P, ClientID sender)
{
	return 0;
}

void IPureServer::OnCL_Connected(IClient* C)
{
}

void IPureServer::OnCL_Disconnected(IClient* C)
{
}

BOOL IPureServer::HasBandwidth(IClient* C)
{
	return FALSE;
}

bool IPureServer::GetClientAddress(ClientID ID, ip_address& Address, DWORD* pPort)
{
	return false;
}

bool IPureServer::DisconnectClient(IClient* C, LPCSTR Reason)
{
	return false;
}

bool IPureServer::DisconnectAddress(const ip_address& Address, LPCSTR reason)
{
	return false;
}

void IPureServer::BanClient(IClient* C, u32 BanTime)
{
}

void IPureServer::BanAddress(const ip_address& Address, u32 BanTime)
{
}

void IPureServer::UnBanAddress(const ip_address& Address)
{
}

void IPureServer::Print_Banned_Addreses()
{
}

void IPureServer::_Recieve(const void* data, u32 data_size, u32 param)
{
	if (data_size >= NET_PacketSizeLimit)
	{
		Msg("! too large packet size[%d] received, DoS attack?", data_size);
		return;
	}

	NET_Packet packet;
	ClientID id;

	id.set(param);
	packet.construct(data, data_size);
	csMessage.Enter();
	u32 result = OnMessage(packet, id);
	csMessage.Leave();

	if (result)
		SendBroadcast(id, packet, result);
}

void ip_address::set(LPCSTR src_string)
{
	u32 buff[4];
	int cnt = sscanf(src_string, "%d.%d.%d.%d", &buff[0], &buff[1], &buff[2], &buff[3]);
	if (cnt == 4)
	{
		m_data.a1 = u8(buff[0] & 0xff);
		m_data.a2 = u8(buff[1] & 0xff);
		m_data.a3 = u8(buff[2] & 0xff);
		m_data.a4 = u8(buff[3] & 0xff);
	}
	else
	{
		Msg("! Bad ipAddress format [%s]", src_string);
		m_data.data = 0;
	}
}

xr_string ip_address::to_string() const
{
	string128 res;
	xr_sprintf(res, sizeof(res), "%d.%d.%d.%d", m_data.a1, m_data.a2, m_data.a3, m_data.a4);
	return res;
}

IClient::IClient(CTimer* timer)
	: stats(timer),
	  server(NULL)
{
	dwTime_LastUpdate = 0;
	flags.bLocal = FALSE;
	flags.bConnected = FALSE;
	flags.bReconnect = FALSE;
	flags.bVerified = TRUE;
}

IClient::~IClient()
{
}

void
IClient::_SendTo_LL(const void* data, u32 size, u32 flags, u32 timeout)
{
	R_ASSERT(server);
	server->IPureServer::SendTo_LL(ID, const_cast<void*>(data), size, flags, timeout);
}
