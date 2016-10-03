
import numpy as np
import phoebe

b = phoebe.Bundle.default_triple()

b.add_dataset('lc', time=np.linspace(0,3,11), dataset='lc01')

b.run_compute(atm='blackbody', ld_func='logarithmic', ld_coeffs=[0,0], reflection_method='none')
