#include "pch.h"
#include "StepTimer.h"

DX::StepTimer::StepTimer()
	: m_elapsedTicks(0),
	m_totalTicks(0),
	m_leftOverTicks(0),
	m_frameCount(0),
	m_framesPerSecond(0),
	m_framesThisSecond(0),
	m_qpcSecondCounter(0),
	m_isFixedTimeStep(false),
	m_targetElapsedTicks(TicksPerSecond / 60) 
{
	if (!QueryPerformanceFrequency(&m_qpcFrequency)) {
		throw std::exception("QueryPerformanceFrequency");
	}

	if (!QueryPerformanceCounter(&m_qpcLastTime)) {
		throw std::exception("QueryPerformanceCounter");
	}

	// Initialize max delta to 1/10 of a second.
	m_qpcMaxDelta = static_cast<uint64_t>(m_qpcFrequency.QuadPart / 10);
}

// After an intentional timing discontinuity (for instance a blocking IO operation)
// call this to avoid having the fixed timestep logic attempt a set of catch-up Update calls.
void DX::StepTimer::ResetElapsedTime() {
	if (!QueryPerformanceCounter(&m_qpcLastTime)) {
		throw std::exception("QueryPerformanceCounter");
	}

	m_leftOverTicks = 0;
	m_framesPerSecond = 0;
	m_framesThisSecond = 0;
	m_qpcSecondCounter = 0;
}
