//////////////////////////////////////////////////////////////////////
// Author :- Nish 
// ServiceHelper.cpp: implementation of the CServiceHelper class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include <wtypes.h>
#include "ServiceHelper.h"

CServiceHelper::CServiceHelper()
{
	m_scm=OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);
}

CServiceHelper::~CServiceHelper()
{
	CloseServiceHandle(m_scm);
}

SERVICEINFO::SERVICEINFO()
{
	lpServiceName=NULL;
	lpDisplayName=NULL;
	lpBinaryPathName=NULL;
	bAutoStart=false;
}

BOOL CServiceHelper::Create()
{
	BOOL suc=false;
	SC_HANDLE tmp;
	if(m_serviceinfo.lpServiceName && 
		m_serviceinfo.lpDisplayName && 
		m_serviceinfo.lpBinaryPathName)
	{	
		tmp=CreateService(m_scm,m_serviceinfo.lpServiceName,
			m_serviceinfo.lpDisplayName,
			SERVICE_ALL_ACCESS,SERVICE_WIN32_OWN_PROCESS,
			m_serviceinfo.bAutoStart?SERVICE_AUTO_START:SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			m_serviceinfo.lpBinaryPathName,
			0,0,0,0,0);
		if(tmp)
		{
			suc=true;
		}
		CloseServiceHandle(tmp);
	}
	return suc;
}

BOOL CServiceHelper::Delete()
{
	BOOL suc=false;
	SC_HANDLE tmp;
	if(m_serviceinfo.lpServiceName)
	{	
		tmp=OpenService(m_scm,m_serviceinfo.lpServiceName,
			SERVICE_ALL_ACCESS);
		if(tmp)
		{
			if(DeleteService(tmp))
			{
				suc=true;
			}
			
		}
		CloseServiceHandle(tmp);
	}
	return suc;

}

BOOL CServiceHelper::Start()
{
	BOOL suc=false;
	SC_HANDLE tmp;
	if(m_serviceinfo.lpServiceName)
	{	
		tmp=OpenService(m_scm,m_serviceinfo.lpServiceName,
			SERVICE_ALL_ACCESS);
		if(tmp)
		{
			if(StartService(tmp,0,NULL))
			{
				suc=true;
			}
			
		}
		CloseServiceHandle(tmp);
	}
	return suc;
}

BOOL CServiceHelper::Stop()
{
	BOOL suc=false;
	SC_HANDLE tmp;
	if(m_serviceinfo.lpServiceName)
	{	
		tmp=OpenService(m_scm,m_serviceinfo.lpServiceName,
			SERVICE_ALL_ACCESS);
		if(tmp)
		{
			SERVICE_STATUS m_SERVICE_STATUS;
			
			if(ControlService(tmp,
				SERVICE_CONTROL_STOP,
				&m_SERVICE_STATUS))
			{
				suc=true;
			}
			
		}
		CloseServiceHandle(tmp);
	}
	return suc;

}

BOOL CServiceHelper::Pause()
{
	BOOL suc=false;
	SC_HANDLE tmp;
	if(m_serviceinfo.lpServiceName)
	{	
		tmp=OpenService(m_scm,m_serviceinfo.lpServiceName,
			SERVICE_ALL_ACCESS);
		if(tmp)
		{
			SERVICE_STATUS m_SERVICE_STATUS;
			
			if(ControlService(tmp,
				SERVICE_CONTROL_PAUSE,
				&m_SERVICE_STATUS))
			{
				suc=true;
			}
			
		}
		CloseServiceHandle(tmp);
	}
	return suc;

}

BOOL CServiceHelper::Continue()
{
	BOOL suc=false;
	SC_HANDLE tmp;
	if(m_serviceinfo.lpServiceName)
	{	
		tmp=OpenService(m_scm,m_serviceinfo.lpServiceName,
			SERVICE_ALL_ACCESS);
		if(tmp)
		{
			SERVICE_STATUS m_SERVICE_STATUS;
			
			if(ControlService(tmp,
				SERVICE_CONTROL_CONTINUE,
				&m_SERVICE_STATUS))
			{
				suc=true;
			}
			
		}
		CloseServiceHandle(tmp);
	}
	return suc;
}

/* Service State -- for CurrentState
#define SERVICE_STOPPED                0x00000001
#define SERVICE_START_PENDING          0x00000002
#define SERVICE_STOP_PENDING           0x00000003
#define SERVICE_RUNNING                0x00000004
#define SERVICE_CONTINUE_PENDING       0x00000005
#define SERVICE_PAUSE_PENDING          0x00000006
#define SERVICE_PAUSED                 0x00000007
*/
BOOL CServiceHelper::QueryStatus(DWORD& dwCurrentState)
{
	BOOL suc=false;
	SC_HANDLE tmp;
	if (m_serviceinfo.lpServiceName)
	{
		tmp=OpenService(m_scm,m_serviceinfo.lpServiceName,
			SERVICE_ALL_ACCESS);
		if(tmp)
		{
			SERVICE_STATUS m_SERVICE_STATUS;

			if (QueryServiceStatus(tmp, 
				&m_SERVICE_STATUS))
			{
				dwCurrentState = m_SERVICE_STATUS.dwCurrentState;
				suc=true;
			}
			
		}		
		CloseServiceHandle(tmp);
	}
	return suc;
}

void CServiceHelper::SetServiceName(LPCTSTR str)
{
	m_serviceinfo.lpServiceName = str;
}

void CServiceHelper::SetServiceDisplayName(LPCTSTR str)
{
	m_serviceinfo.lpDisplayName = str;
}

void CServiceHelper::SetServicePath(LPCTSTR str)
{
	m_serviceinfo.lpBinaryPathName = str;
}

void CServiceHelper::SetAutoStart(BOOL b)
{
	m_serviceinfo.bAutoStart = b;
}