
#include "xqhistory.hh"


XiangqiHistory::XiangqiHistory() : History() {
}

const XiangqiState& XiangqiHistory::operator[](int p) const {
	return *static_cast<XiangqiState*>(this->getHistory()[p]);
}
