#include "cloginclient.h"
#include "ePacketType.h"

CLoginClient::CLoginClient() : CRoseClient()
{
	m_Log.SetIdentity( "CLoginClient" );
}

CLoginClient::CLoginClient( tcp::socket _sock ) : CRoseClient( std::move( _sock ) )
{
	m_Log.SetIdentity( "CLoginClient" );
}

bool CLoginClient::HandlePacket( uint8_t* _buffer )
{
	CPacket* pak = (CPacket*)_buffer;
	switch ( pak->Header.Command )
	{
	default:
	{
		CRoseClient::HandlePacket( _buffer );
		return false;
	}
	}
	return true;
}

void CLoginClient::OnReceived( uint8_t* _buffer, uint16_t _size )
{
	CRoseClient::OnReceived( _buffer, _size );
}
