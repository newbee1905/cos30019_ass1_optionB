#include "search/dfs.hh"

int kd::DFS::step() {
	if (m_frontier.empty())
		return 1;
	m_cur         = m_frontier.top();
	m_grid[m_cur] = BlockState::VISIT;
	m_frontier.pop();

	if (m_cur == m_goal)
		return 1;

	for (const auto &c : kd::DfsCellAdjs) {
		const auto ncell = m_cur + c.fst;
		if (!m_grid.cell_valid(ncell))
			continue;
		if (m_grid.at(ncell) >= BlockState::BLOCK)
			continue;
		/* grid[ncell] = BlockState::VISIT; */
		m_frontier.push(ncell);
		++m_nnodes;
		m_parent[ncell] = kd::pair<kd::Cell, Action>{m_cur, c.sec};
	}
	return 0;
}

int kd::DFS::run() {
	while (step() == 0)
		;
	if (m_cur != m_goal)
		return 1;
	for (; m_cur != m_agent.pos();
	     m_path.emplace_back(m_parent[m_cur].sec), m_cur = m_parent[m_cur].fst)
		;
	return 0;
}

const int &kd::DFS::nnodes() { return m_nnodes; }
const std::vector<Action> &kd::DFS::path() { return m_path; }

void kd::DFS::print_path() {
	for (std::size_t i = m_path.size(); i-- > 0; fmt::print("{}; ", this->m_path[i]))
		;
}
