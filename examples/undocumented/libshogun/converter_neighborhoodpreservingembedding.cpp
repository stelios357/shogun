/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Sergey Lisitsyn, Heiko Strathmann, Soeren Sonnenburg, Pan Deng
 */

#include <shogun/lib/config.h>
#ifdef USE_GPL_SHOGUN
#include <shogun/features/DenseFeatures.h>
#include <shogun/converter/NeighborhoodPreservingEmbedding.h>
#include <shogun/mathematics/Math.h>

using namespace shogun;

int main(int argc, char** argv)
{
	int N = 100;
	int dim = 3;
	float64_t* matrix = new double[N*dim];
	for (int i=0; i<N*dim; i++)
		matrix[i] = std::sin((i / float64_t(N * dim)) * 3.14);

	CDenseFeatures<double>* features = new CDenseFeatures<double>(SGMatrix<double>(matrix,dim,N));
	SG_REF(features);
	CNeighborhoodPreservingEmbedding* npe = new CNeighborhoodPreservingEmbedding();
	npe->set_target_dim(2);
	npe->set_k(15);
	npe->get_global_parallel()->set_num_threads(4);
	auto embedding = npe->transform(features);
	SG_UNREF(embedding);
	SG_UNREF(npe);
	SG_UNREF(features);
	return 0;
}
#else //USE_GPL_SHOGUN
int main(int argc, char** argv)
{
	return 0;
}
#endif //USE_GPL_SHOGUN
