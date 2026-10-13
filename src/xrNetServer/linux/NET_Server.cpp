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
