#include "PrimAlgorithm.cpp"
#include "KruskalAlgorithm.cpp"
//#include "MSTAlgorithm.hpp"

class MSTFactory {
public:
    enum MSTAlgo {
        PRIM,
        KRUSKAL
    };

    static MSTAlgorithm* createMSTSolver(int type) {
        MSTAlgorithm* solver = NULL;

        switch (type) {
            case PRIM:
                solver = new PrimAlgorithm();
                break;
            case KRUSKAL:
                solver = new KruskalAlgorithm();
                break;
            default:
                std::cerr << "Invalid algorithm type!" << std::endl;
                return solver;
        }

        // Calculate MST using the selected algorithm
        //solver->calculateMST(graph);
        return solver;
        //delete solver;
    }
};