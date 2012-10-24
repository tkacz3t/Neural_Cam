#pragma once

class CTrackerPod
{
public:
	CTrackerPod() { }
	virtual ~CTrackerPod() { }

	// Compatibility functions for old applications
	virtual bool initialize(char *client_id, bool b_show_on_event)				= 0;

	virtual bool begin_enum_device()				= 0;
	virtual bool enum_reset()						= 0;
	virtual long enum_next(char *ver)				= 0;

	virtual bool use_device(long devid, bool bReportBack = true)				= 0;
	virtual bool move_to(float x, float y)			= 0;
	virtual bool move_by(float x, float y)			= 0;
	virtual long control(char cmd[1024])			= 0;

	virtual bool get_pos(float *x, float *y)		= 0;
	virtual bool get_info(char info[1024])			= 0;

	virtual void show_on_event(bool show)			= 0;
	virtual void show_manager(bool show)			= 0;
	virtual bool is_manager_visible()				= 0;

	virtual void ReleaseTrackerPod()				= 0;
	static CTrackerPod* CreateTrackerPod();
	virtual long GetDeviceInUse()					= 0;

	virtual bool GetDeviceInfo(long lDevID, char info[1024]) = 0;
	virtual int GetDeviceCount() = 0;

	virtual bool UseNotification(HWND hWnd, UINT uiMessage, int nNotificationIndex) = 0;



	// New functions

	// Sets whether the calls will be blocking or non-blocking
	virtual void SetBlocking(bool bBlocking)		= 0;
};