#!/usr/bin/env python
import re
import time
from tools.multiclass_shared import prepare_data

# run with toy data
[traindat, label_traindat, testdat, label_testdat] = prepare_data()
# run with opt-digits if available
#[traindat, label_traindat, testdat, label_testdat] = prepare_data(False)

parameter_list = [[traindat,testdat,label_traindat,label_testdat,2.1,1,1e-5]]

def classifier_multiclass_ecoc (fm_train_real=traindat,fm_test_real=testdat,label_train_multiclass=label_traindat,label_test_multiclass=label_testdat,lawidth=2.1,C=1,epsilon=1e-5):

	import shogun
	from shogun import ECOCStrategy, LinearMulticlassMachine
	from shogun import MulticlassAccuracy
	from shogun import MulticlassLabels
	from shogun import ecoc_encoder
	from shogun import ecoc_decoder
	import shogun as sg

	def nonabstract_class(name):
		try:
		    getattr(shogun, name)()
		except TypeError:
		    return False
		return True

	encoders = [x for x in dir(shogun)
		    if re.match(r'ECOC.+Encoder', x) and nonabstract_class(x)]
	decoders = [x for x in dir(shogun)
		    if re.match(r'ECOC.+Decoder', x) and nonabstract_class(x)]

	fea_train = sg.features(fm_train_real)
	fea_test  = sg.features(fm_test_real)
	gnd_train = MulticlassLabels(label_train_multiclass)
	if label_test_multiclass is None:
		gnd_test = None
	else:
		gnd_test = MulticlassLabels(label_test_multiclass)

	base_classifier = sg.machine("LibLinear",
								liblinear_solver_type="L2R_L2LOSS_SVC",
								use_bias=True)

	#print('Testing with %d encoders and %d decoders' % (len(encoders), len(decoders)))
	#print('-' * 70)
	#format_str = '%%15s + %%-10s  %%-10%s %%-10%s %%-10%s'
	#print((format_str % ('s', 's', 's')) % ('encoder', 'decoder', 'codelen', 'time', 'accuracy'))

	def run_ecoc(ier, idr):
		encoder = ecoc_encoder(encoders[ier])
		decoder = ecoc_decoder(decoders[idr])

		# whether encoder is data dependent
		if encoder.has('labels'):
		    encoder.put('labels', gnd_train)
		    encoder.put('features', fea_train)

		strategy = ECOCStrategy(encoder, decoder)
		classifier = LinearMulticlassMachine(strategy, fea_train, base_classifier, gnd_train)
		classifier.train()
		label_pred = classifier.apply(fea_test)
		if gnd_test is not None:
		    evaluator = MulticlassAccuracy()
		    acc = evaluator.evaluate(label_pred, gnd_test)
		else:
		    acc = None

		return (classifier.get_num_machines(), acc)


	for ier in range(len(encoders)):
		for idr in range(len(decoders)):
		    t_begin = time.clock()
		    (codelen, acc) = run_ecoc(ier, idr)
		    if acc is None:
		        acc_fmt = 's'
		        acc = 'N/A'
		    else:
		        acc_fmt = '.4f'

		    t_elapse = time.clock() - t_begin
		    #print((format_str % ('d', '.3f', acc_fmt)) %
		    #        (encoders[ier][4:-7], decoders[idr][4:-7], codelen, t_elapse, acc))

if __name__=='__main__':
    print('MulticlassECOC')
    classifier_multiclass_ecoc(*parameter_list[0])

