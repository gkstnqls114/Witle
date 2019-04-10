#include "stdafx.h"
#include "PointerForNetwork.h"


SEND_BUFFER PointerForNetwork::m_SendBuffer;

XMFLOAT4X4* PointerForNetwork::m_pPlayerWorld{ nullptr };
UCHAR* PointerForNetwork::m_pKeyBuffer{ nullptr };