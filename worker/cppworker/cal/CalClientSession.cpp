#include "CalMessages.h"
#include "CalClient.h"
#include "CalClientSession.h"
#include "CalConst.h"

#include <stdio.h>
#include <string.h>
#include <memory>
#include <utility/encoding/base64.h>

CalClientSession::CalClientSession()
	: m_session_transaction(NULL),
	  m_status(CAL::get_trans_ok())
{
}

CalClientSession::~CalClientSession()
{
	end_session();
}

void CalClientSession::start_session(const std::string &_type, const std::string& _session_name)
{
	end_session();

	m_session_transaction = std::shared_ptr<CalTransaction>(new CalTransaction(_type));
	m_session_transaction->SetName(_session_name);
}

void CalClientSession::start_session(const std::string &_session_name)
{
	start_session(CAL::TRANS_TYPE_CLIENT, _session_name);
}

void CalClientSession::end_session()
{
	if (is_session_active()) {
		m_session_transaction->Completed(m_status);
		m_session_transaction = std::shared_ptr<CalTransaction>(NULL);
		m_status = CAL::TRANS_OK;
	}
}

bool CalClientSession::is_session_active() const
{
	return (m_session_transaction.get() != NULL);
}

void CalClientSession::set_status(const std::string &_status)
{
	m_status = _status;
}
void CalClientSession::generate_trace_log_id(std::string &_pool)
{
        char time_stamp[12];
        CalTimeOfDay::Now(time_stamp);
        char milli_sec[2];
        strcpy(milli_sec, strrchr(time_stamp,'.') + 1);
        time_t currTime=time(NULL);
        char r_log_id[70];
        sprintf(r_log_id,"%llx", ((unsigned long long) currTime) *1000 + atoi (milli_sec)* 10);
        m_log_id = r_log_id;
        m_log_id = m_log_id + "-" + _pool.c_str();
}
