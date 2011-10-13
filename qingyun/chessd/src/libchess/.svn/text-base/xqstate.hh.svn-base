
#ifndef XIANGQI_STATE_HH
#define XIANGQI_STATE_HH

#include <vector>
#include <string>
#include "State.hh"

class XiangqiState : public State {
	private:

	public:
		
//	public:
		/*! \brief Initializes whith the Internatinal Chess begining state*/
		XiangqiState();

		XiangqiState(const std::string& FEN);

		virtual std::string FEN() const;

		bool operator==(const XiangqiState& s) const;
};
#endif
