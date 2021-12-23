#include <vector>
#include <queue>

using namespace std;

typedef long long llong;

class edge {
public:
	edge(int dst, int weight);
	int get_dst() const;
	int get_weight() const;
private:
	int dst_;
	int weight_;
};

edge::edge(int dst, int weight) :
	dst_(dst),
	weight_(weight) {}

int edge::get_dst() const {
	return dst_;
}

int edge::get_weight() const {
	return weight_;
}

class graph {
public:
	graph();
	graph(int v, bool undirected = false);

	void operator() (int v, bool undirected = false);

	void set_number_of_vertices(int v);
	void add_edge(int src, int dst, int weight);

	vector<int> dijkstra(int src);
	vector<long> bellman_ford(int src);
	vector<vector<llong>> floyd_warshall();
private:
	graph(bool is_undirected);

	vector<vector<edge>> edges_;
	bool is_undirected_;

};

graph::graph() :
	graph(false) {}

graph::graph(bool is_undirected) :
	is_undirected_(is_undirected) {}

graph::graph(int v, bool undirected = false) :
	graph(undirected) {
	edges_.assign(v, vector<edge>());
}

void graph::operator() (int v, bool undirected = false) {
	set_number_of_vertices(v);
	is_undirected_ = undirected;
}

void graph::set_number_of_vertices(int v) {
	edges_.assign(v, vector<edge>());
}

void graph::add_edge(int src, int dst, int weight) {
	edges_[src].push_back(edge(dst, weight));
	if (is_undirected_) {
		edges_[dst].push_back(edge(src, weight));
	}
}

vector<int> graph::dijkstra(int src) {
	vector<int> dist(edges_.size(), INT_MAX);
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

	dist[src] = 0;
	pq.push(make_pair(0, src));

	while (!pq.empty()) {
		pair<int, int> cur = pq.top();
		pq.pop();

		for (int i = 0; i < edges_[cur.second].size(); i++) {
			const edge& e = edges_[cur.second][i];
			if (dist[cur.second] + e.get_weight() < dist[e.get_dst()]) {
				dist[e.get_dst()] = dist[cur.second] + e.get_weight();
				pq.push(make_pair(dist[e.get_dst()], e.get_dst()));
			}
		}
	}

	return dist;
}

vector<long> graph::bellman_ford(int src) {
	vector<long> dist(edges_.size(), INT_MAX);
	bool update = false;

	dist[src] = 0;

	for (int i = 0; i < edges_.size(); i++) {
		update = false;
		for (int j = 0; j < edges_.size(); j++) {
			for (const edge& e : edges_[j]) {
				if (dist[j] != INT_MAX && (dist[e.get_dst()] > dist[j] + e.get_weight())) {
					dist[e.get_dst()] = dist[j] + e.get_weight();
					update = true;
				}
			}
		}
	}

	if (update) {
		return vector<long>();
	}

	return dist;
}

vector<vector<llong>> graph::floyd_warshall() {
	vector<vector<llong>> dist(edges_.size(), vector<llong>(edges_.size(), INT_MAX));

	for (int i = 0; i < edges_.size(); i++) {
		for (const edge& e : edges_[i]) {
			dist[i][e.get_dst()] = min(dist[i][e.get_dst()], static_cast<llong>(e.get_weight()));
		}
	}

	for (int i = 0; i < edges_.size(); i++) {
		dist[i][i] = 0;
	}

	for (int k = 0; k < edges_.size(); k++) {
		for (int i = 0; i < edges_.size(); i++) {
			for (int j = 0; j < edges_.size(); j++) {
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}

	return dist;
}
