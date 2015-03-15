#pragma once
#include <vector>
#include <plog/Appenders/IAppender.h>
#include <plog/Util.h>

namespace plog
{
    template<int instance>
    class Logger : public util::Singleton<Logger<instance> >, public IAppender
    {
    public:
        Logger(Severity maxSeverity = none) : m_maxSeverity(maxSeverity)
        {
        }

        Logger& addAppender(IAppender* appender)
        {
            assert(appender != this);
            assert(std::find(m_appenders.begin(), m_appenders.end(), appender) == m_appenders.end());

            m_appenders.push_back(appender);
            return *this;
        }

        Severity getMaxSeverity() const
        {
            return m_maxSeverity;
        }

        void setMaxSeverity(Severity severity)
        {
            m_maxSeverity = severity;
        }

        bool checkSeverity(Severity severity) const
        {
            return severity <= m_maxSeverity;
        }

        virtual void write(const Record& record)
        {
            if (checkSeverity(record.m_severity))
            {
                *this += record;
            }
        }

        void operator+=(const Record& record)
        {
            for (std::vector<IAppender*>::iterator it = m_appenders.begin(); it != m_appenders.end(); ++it)
            {
                (*it)->write(record);
            }
        }

    private:
        Severity m_maxSeverity;
        std::vector<IAppender*> m_appenders;
    };

    template<int instance>
    inline Logger<instance>* get()
    {
        return Logger<instance>::getInstance();
    }

    inline Logger<0>* get()
    {
        return Logger<0>::getInstance();
    }
}
