#ifndef H_ProfilingTimer
#define H_ProfilingTimer
#ifdef USE_PROFILER
#include <chrono>


namespace Prifiler
{
	class CallbackBase
	{
	public:
		virtual void Execute(double seconds) = 0;
	};
	


	template<typename Func>
	class Callback: public CallbackBase
	{
		Func m_callback;
	public:
		Callback(Func callback)
			:m_callback(callback)
		{}

		void Execute(double seconds)
		{
			m_callback(seconds);
		}
	};



	class ProfilingTimer
	{
		CallbackBase* 		m_MesurmentFinishCallBack = 0;


		std::chrono::time_point<std::chrono::high_resolution_clock> m_tStart;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_tStop;


	public:

		template<typename Func>
		ProfilingTimer(Func f)
		{
			m_MesurmentFinishCallBack = new Callback<Func>(f);

			m_tStart = std::chrono::high_resolution_clock::now();
		}

		~ProfilingTimer();
		

	};
}

#endif
#endif