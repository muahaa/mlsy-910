/****************************************************************************************************/
//驱动
#include "LPC23xx.h"
#include "type.h"
#include "irq.h"
#include "target.h"
#include "absacc.h"
#include "timer.h"
//#include "ex_int.h"
#include "rtc.h"
#include "can.h"
#include "uart.h"
#include "lcd1602.h"
//电梯
#define  Menu_Ex
#include "IO.h"
#include "Call.h"
#include "Lift.h"
#include "Flash.h"
#include "Menu.h"
#include "LiftCan.h"
#include "main.h"
/****************************************************************************************************/
#define KEY_OK	    0x01
#define KEY_ESC	    0x02
#define KEY_UP	    0x04
#define KEY_DOWN	  0x08
#define KEY_NULL	  0x00
/*--------------------------------------------------------------------------------------------------*/
#define MENU_000    0
#define MENU_001   (MENU_000+1)

#define MENU_100   (MENU_001+1)   					
#define MENU_101   (MENU_100+1) 					
#define MENU_102   (MENU_101+1)				
#define MENU_103   (MENU_102+1)					
#define MENU_104   (MENU_103+1) 				
#define MENU_105   (MENU_104+1)   					
#define MENU_106   (MENU_105+1)   					
#define MENU_107   (MENU_106+1)   					
#define MENU_108   (MENU_107+1)   					
#define MENU_109   (MENU_108+1)   					
#define MENU_110   (MENU_109+1)   					
#define MENU_111   (MENU_110+1)   					
#define MENU_112   (MENU_111+1)   					
#define MENU_113   (MENU_112+1)   					
#define MENU_114   (MENU_113+1)   					
#define MENU_115   (MENU_114+1)   					
#define MENU_116   (MENU_115+1)   					
#define MENU_117   (MENU_116+1)   					
#define MENU_118   (MENU_117+1)   					
#define MENU_119   (MENU_118+1)   					
#define MENU_120   (MENU_119+1)   					
#define MENU_121   (MENU_120+1)   					
#define MENU_122   (MENU_121+1)   					
#define MENU_123   (MENU_122+1)   					
#define MENU_124   (MENU_123+1)   					
#define MENU_125   (MENU_124+1)   					
#define MENU_126   (MENU_125+1)
#define MENU_127   (MENU_126+1)
#define MENU_128   (MENU_127+1)
#define MENU_129   (MENU_128+1)
#define MENU_130   (MENU_129+1)
#define MENU_131   (MENU_130+1)
#define MENU_132   (MENU_131+1)
#define MENU_133   (MENU_132+1)
#define MENU_134   (MENU_133+1)
#define MENU_135   (MENU_134+1)
#define MENU_136   (MENU_135+1)
#define MENU_137   (MENU_136+1)
#define MENU_138   (MENU_137+1)
#define MENU_139   (MENU_138+1)
#define MENU_140   (MENU_139+1)
#define MENU_141   (MENU_140+1)
#define MENU_142   (MENU_141+1)
#define MENU_143   (MENU_142+1)
#define MENU_144   (MENU_143+1)
#define MENU_145   (MENU_144+1)
#define MENU_146   (MENU_145+1)
#define MENU_147   (MENU_146+1)
#define MENU_148   (MENU_147+1)
#define MENU_149   (MENU_148+1)
#define MENU_150   (MENU_149+1)
#define MENU_151   (MENU_150+1)
#define MENU_152   (MENU_151+1)

#define MENU_153   (MENU_152+1)
#define MENU_154   (MENU_153+1)
#define MENU_155   (MENU_154+1)
#define MENU_156   (MENU_155+1)
#define MENU_157   (MENU_156+1)
#define MENU_158   (MENU_157+1)
#define MENU_159   (MENU_158+1)
#define MENU_160   (MENU_159+1)

#define MENU_161   (MENU_160+1)
#define MENU_162   (MENU_161+1)
#define MENU_163   (MENU_162+1)
#define MENU_164   (MENU_163+1)
#define MENU_165   (MENU_164+1)
#define MENU_166   (MENU_165+1)
#define MENU_167   (MENU_166+1)
#define MENU_168   (MENU_167+1)
#define MENU_169   (MENU_168+1)
#define MENU_170   (MENU_169+1)
#define MENU_171   (MENU_170+1)
#define MENU_172   (MENU_171+1)
#define MENU_173   (MENU_172+1)
#define MENU_174   (MENU_173+1)
#define MENU_175   (MENU_174+1)
#define MENU_176   (MENU_175+1)

#define MENU_200   (MENU_176+1)
#define MENU_201   (MENU_200+1)
#define MENU_202   (MENU_201+1)
#define MENU_203   (MENU_202+1)
#define MENU_204   (MENU_203+1)
#define MENU_205   (MENU_204+1)
#define MENU_206   (MENU_205+1)
#define MENU_207   (MENU_206+1)
#define MENU_208   (MENU_207+1)
#define MENU_209   (MENU_208+1)
#define MENU_210   (MENU_209+1)
#define MENU_211   (MENU_210+1)
#define MENU_212   (MENU_211+1)
#define MENU_213   (MENU_212+1)
#define MENU_214   (MENU_213+1)
#define MENU_215   (MENU_214+1)
#define MENU_216   (MENU_215+1)
#define MENU_217   (MENU_216+1)
#define MENU_218   (MENU_217+1)
#define MENU_219   (MENU_218+1)
#define MENU_220   (MENU_219+1)
#define MENU_221   (MENU_220+1)
#define MENU_222   (MENU_221+1)
#define MENU_223   (MENU_222+1)
#define MENU_224   (MENU_223+1)
#define MENU_225   (MENU_224+1)
#define MENU_226   (MENU_225+1)
#define MENU_227   (MENU_226+1)
#define MENU_228   (MENU_227+1)
#define MENU_229   (MENU_228+1)
#define MENU_230   (MENU_229+1)
#define MENU_231   (MENU_230+1)
#define MENU_232   (MENU_231+1)
#define MENU_233   (MENU_232+1)
#define MENU_234   (MENU_233+1)
#define MENU_235   (MENU_234+1)
#define MENU_236   (MENU_235+1)
#define MENU_237   (MENU_236+1)
#define MENU_238   (MENU_237+1)
#define MENU_239   (MENU_238+1)
#define MENU_240   (MENU_239+1)
#define MENU_241   (MENU_240+1)
#define MENU_242   (MENU_241+1)
#define MENU_243   (MENU_242+1)
#define MENU_244   (MENU_243+1)
#define MENU_245   (MENU_244+1)
#define MENU_246   (MENU_245+1)
#define MENU_247   (MENU_246+1)
#define MENU_248   (MENU_247+1)
#define MENU_249   (MENU_248+1)
#define MENU_250   (MENU_249+1)
#define MENU_251   (MENU_250+1)
#define MENU_252   (MENU_251+1)
#define MENU_253   (MENU_252+1)
#define MENU_254   (MENU_253+1)
#define MENU_255   (MENU_254+1)
   				
#define MENU_300   (MENU_255+1)
#define MENU_301   (MENU_300+1)  				 				
#define MENU_302   (MENU_301+1)   					
#define MENU_303   (MENU_302+1)   					
#define MENU_304   (MENU_303+1)   					
#define MENU_305   (MENU_304+1)  				
#define MENU_306   (MENU_305+1)
#define MENU_307   (MENU_306+1)
#define MENU_308   (MENU_307+1)
#define MENU_309   (MENU_308+1)
#define MENU_310   (MENU_309+1)
#define MENU_311   (MENU_310+1)
#define MENU_312   (MENU_311+1)
#define MENU_313   (MENU_312+1)
#define MENU_314   (MENU_313+1)
#define MENU_315   (MENU_314+1)
#define MENU_316   (MENU_315+1)
#define MENU_317   (MENU_316+1)
#define MENU_318   (MENU_317+1)
#define MENU_319   (MENU_318+1)
#define MENU_320   (MENU_319+1)
#define MENU_321   (MENU_320+1)
#define MENU_322   (MENU_321+1)
#define MENU_323   (MENU_322+1)
#define MENU_324   (MENU_323+1)
#define MENU_325   (MENU_324+1)
#define MENU_326   (MENU_325+1)
#define MENU_327   (MENU_326+1)
#define MENU_328   (MENU_327+1)
#define MENU_329   (MENU_328+1)
#define MENU_330   (MENU_329+1)
#define MENU_331   (MENU_330+1)
#define MENU_332   (MENU_331+1)
#define MENU_333   (MENU_332+1)
#define MENU_334   (MENU_333+1)
#define MENU_335   (MENU_334+1)
#define MENU_336   (MENU_335+1)
#define MENU_337   (MENU_336+1)
#define MENU_338   (MENU_337+1)
#define MENU_339   (MENU_338+1)
#define MENU_340   (MENU_339+1)
#define MENU_341   (MENU_340+1)
#define MENU_342   (MENU_341+1)
#define MENU_343   (MENU_342+1)
#define MENU_344   (MENU_343+1)
#define MENU_345   (MENU_344+1)
#define MENU_346   (MENU_345+1)
#define MENU_347   (MENU_346+1)
#define MENU_348   (MENU_347+1)
#define MENU_349   (MENU_348+1)
#define MENU_350   (MENU_349+1)
#define MENU_351   (MENU_350+1)
#define MENU_352   (MENU_351+1)
#define MENU_353   (MENU_352+1)
#define MENU_354   (MENU_353+1)
#define MENU_355   (MENU_354+1)

#define MENU_400   (MENU_355+1)   				
#define MENU_401   (MENU_400+1)
#define MENU_402   (MENU_401+1)
#define MENU_403   (MENU_402+1)  				
#define MENU_404   (MENU_403+1)   				
#define MENU_405   (MENU_404+1)
#define MENU_406   (MENU_405+1)
#define MENU_407   (MENU_406+1)
#define MENU_408   (MENU_407+1)
#define MENU_409   (MENU_408+1)
#define MENU_410   (MENU_409+1)
#define MENU_411   (MENU_410+1)
#define MENU_412   (MENU_411+1)
#define MENU_413   (MENU_412+1)
#define MENU_414   (MENU_413+1)
#define MENU_415   (MENU_414+1)
#define MENU_416   (MENU_415+1)
#define MENU_417   (MENU_416+1)
#define MENU_418   (MENU_417+1)
#define MENU_419   (MENU_418+1)
#define MENU_420   (MENU_419+1)
#define MENU_421   (MENU_420+1)
#define MENU_422   (MENU_421+1)
#define MENU_423   (MENU_422+1)
#define MENU_424   (MENU_423+1)
#define MENU_425   (MENU_424+1)
#define MENU_426   (MENU_425+1)
#define MENU_427   (MENU_426+1)
#define MENU_428   (MENU_427+1)
#define MENU_429   (MENU_428+1)
#define MENU_430   (MENU_429+1)
#define MENU_431   (MENU_430+1)
#define MENU_432   (MENU_431+1)
#define MENU_433   (MENU_432+1)
#define MENU_434   (MENU_433+1)
#define MENU_435   (MENU_434+1)
#define MENU_436   (MENU_435+1)
#define MENU_437   (MENU_436+1)
#define MENU_438   (MENU_437+1)
#define MENU_439   (MENU_438+1)
#define MENU_440   (MENU_439+1)
#define MENU_441   (MENU_440+1)
#define MENU_442   (MENU_441+1)
#define MENU_443   (MENU_442+1)
#define MENU_444   (MENU_443+1)
#define MENU_445   (MENU_444+1)
#define MENU_446   (MENU_445+1)
#define MENU_447   (MENU_446+1)
#define MENU_448   (MENU_447+1)
#define MENU_449   (MENU_448+1)
#define MENU_450   (MENU_449+1)
#define MENU_451   (MENU_450+1)
#define MENU_452   (MENU_451+1)
#define MENU_453   (MENU_452+1)
#define MENU_454   (MENU_453+1)
#define MENU_455   (MENU_454+1)

#define MENU_500   (MENU_455+1)   				
#define MENU_501   (MENU_500+1)
#define MENU_502   (MENU_501+1)
#define MENU_503   (MENU_502+1)  				
#define MENU_504   (MENU_503+1)   				
#define MENU_505   (MENU_504+1)
#define MENU_506   (MENU_505+1)
#define MENU_507   (MENU_506+1)
#define MENU_508   (MENU_507+1)
#define MENU_509   (MENU_508+1)
#define MENU_510   (MENU_509+1)
#define MENU_511   (MENU_510+1)
#define MENU_512   (MENU_511+1)
#define MENU_513   (MENU_512+1)
#define MENU_514   (MENU_513+1)
#define MENU_515   (MENU_514+1)
#define MENU_516   (MENU_515+1)
#define MENU_517   (MENU_516+1)
#define MENU_518   (MENU_517+1)
#define MENU_519   (MENU_518+1)
#define MENU_520   (MENU_519+1)
#define MENU_521   (MENU_520+1)
#define MENU_522   (MENU_521+1)
#define MENU_523   (MENU_522+1)
#define MENU_524   (MENU_523+1)
#define MENU_525   (MENU_524+1)
#define MENU_526   (MENU_525+1)
#define MENU_527   (MENU_526+1)
#define MENU_528   (MENU_527+1)
#define MENU_529   (MENU_528+1)
#define MENU_530   (MENU_529+1)
#define MENU_531   (MENU_530+1)
#define MENU_532   (MENU_531+1)
#define MENU_533   (MENU_532+1)
#define MENU_534   (MENU_533+1)
#define MENU_535   (MENU_534+1)
#define MENU_536   (MENU_535+1)
#define MENU_537   (MENU_536+1)
#define MENU_538   (MENU_537+1)
#define MENU_539   (MENU_538+1)
#define MENU_540   (MENU_539+1)
#define MENU_541   (MENU_540+1)
#define MENU_542   (MENU_541+1)
#define MENU_543   (MENU_542+1)
#define MENU_544   (MENU_543+1)
#define MENU_545   (MENU_544+1)
#define MENU_546   (MENU_545+1)
#define MENU_547   (MENU_546+1)
#define MENU_548   (MENU_547+1)
#define MENU_549   (MENU_548+1)
#define MENU_550   (MENU_549+1)
#define MENU_551   (MENU_550+1)
#define MENU_552   (MENU_551+1)
#define MENU_553   (MENU_552+1)
#define MENU_554   (MENU_553+1)
#define MENU_555   (MENU_554+1)

#define MENU_600   (MENU_555+1)   				
#define MENU_601   (MENU_600+1)
#define MENU_602   (MENU_601+1)
#define MENU_603   (MENU_602+1)  				
#define MENU_604   (MENU_603+1)   				
#define MENU_605   (MENU_604+1)
#define MENU_606   (MENU_605+1)
#define MENU_607   (MENU_606+1)
#define MENU_608   (MENU_607+1)
#define MENU_609   (MENU_608+1)
#define MENU_610   (MENU_609+1)
#define MENU_611   (MENU_610+1)
#define MENU_612   (MENU_611+1)
#define MENU_613   (MENU_612+1)
#define MENU_614   (MENU_613+1)
#define MENU_615   (MENU_614+1)
#define MENU_616   (MENU_615+1)
#define MENU_617   (MENU_616+1)
#define MENU_618   (MENU_617+1)
#define MENU_619   (MENU_618+1)
#define MENU_620   (MENU_619+1)
#define MENU_621   (MENU_620+1)
#define MENU_622   (MENU_621+1)
#define MENU_623   (MENU_622+1)
#define MENU_624   (MENU_623+1)
#define MENU_625   (MENU_624+1)
#define MENU_626   (MENU_625+1)
#define MENU_627   (MENU_626+1)
#define MENU_628   (MENU_627+1)
#define MENU_629   (MENU_628+1)
#define MENU_630   (MENU_629+1)
#define MENU_631   (MENU_630+1)
#define MENU_632   (MENU_631+1)
#define MENU_633   (MENU_632+1)
#define MENU_634   (MENU_633+1)
#define MENU_635   (MENU_634+1)
#define MENU_636   (MENU_635+1)
#define MENU_637   (MENU_636+1)
#define MENU_638   (MENU_637+1)
#define MENU_639   (MENU_638+1)
#define MENU_640   (MENU_639+1)
#define MENU_641   (MENU_640+1)
#define MENU_642   (MENU_641+1)
#define MENU_643   (MENU_642+1)
#define MENU_644   (MENU_643+1)
#define MENU_645   (MENU_644+1)
#define MENU_646   (MENU_645+1)
#define MENU_647   (MENU_646+1)
#define MENU_648   (MENU_647+1)
#define MENU_649   (MENU_648+1)
#define MENU_650   (MENU_649+1)
#define MENU_651   (MENU_650+1)
#define MENU_652   (MENU_651+1)
#define MENU_653   (MENU_652+1)
#define MENU_654   (MENU_653+1)
#define MENU_655   (MENU_654+1)

#define MENU_700   (MENU_655+1)   				
#define MENU_701   (MENU_700+1)
#define MENU_702   (MENU_701+1)
#define MENU_703   (MENU_702+1)  				
#define MENU_704   (MENU_703+1)   				
#define MENU_705   (MENU_704+1)
#define MENU_706   (MENU_705+1)
#define MENU_707   (MENU_706+1)
#define MENU_708   (MENU_707+1)
#define MENU_709   (MENU_708+1)
#define MENU_710   (MENU_709+1)
#define MENU_711   (MENU_710+1)
#define MENU_712   (MENU_711+1)
#define MENU_713   (MENU_712+1)
#define MENU_714   (MENU_713+1)
#define MENU_715   (MENU_714+1)
#define MENU_716   (MENU_715+1)
#define MENU_717   (MENU_716+1)
#define MENU_718   (MENU_717+1)
#define MENU_719   (MENU_718+1)
#define MENU_720   (MENU_719+1)
#define MENU_721   (MENU_720+1)
#define MENU_722   (MENU_721+1)
#define MENU_723   (MENU_722+1)
#define MENU_724   (MENU_723+1)
#define MENU_725   (MENU_724+1)
#define MENU_726   (MENU_725+1)
#define MENU_727   (MENU_726+1)
#define MENU_728   (MENU_727+1)
#define MENU_729   (MENU_728+1)
#define MENU_730   (MENU_729+1)
#define MENU_731   (MENU_730+1)
#define MENU_732   (MENU_731+1)
#define MENU_733   (MENU_732+1)
#define MENU_734   (MENU_733+1)
#define MENU_735   (MENU_734+1)
#define MENU_736   (MENU_735+1)
#define MENU_737   (MENU_736+1)
#define MENU_738   (MENU_737+1)
#define MENU_739   (MENU_738+1)
#define MENU_740   (MENU_739+1)
#define MENU_741   (MENU_740+1)
#define MENU_742   (MENU_741+1)
#define MENU_743   (MENU_742+1)
#define MENU_744   (MENU_743+1)
#define MENU_745   (MENU_744+1)
#define MENU_746   (MENU_745+1)
#define MENU_747   (MENU_746+1)
#define MENU_748   (MENU_747+1)
#define MENU_749   (MENU_748+1)
#define MENU_750   (MENU_749+1)
#define MENU_751   (MENU_750+1)
#define MENU_752   (MENU_751+1)
#define MENU_753   (MENU_752+1)
#define MENU_754   (MENU_753+1)
#define MENU_755   (MENU_754+1)

#define MENU_800   (MENU_755+1)  				
#define MENU_801   (MENU_800+1)
#define MENU_802   (MENU_801+1)
#define MENU_803   (MENU_802+1)  				
#define MENU_804   (MENU_803+1)   				
#define MENU_805   (MENU_804+1)
#define MENU_806   (MENU_805+1)
#define MENU_807   (MENU_806+1)
#define MENU_808   (MENU_807+1)
#define MENU_809   (MENU_808+1)
#define MENU_810   (MENU_809+1)
#define MENU_811   (MENU_810+1)
#define MENU_812   (MENU_811+1)
#define MENU_813   (MENU_812+1)
#define MENU_814   (MENU_813+1)
#define MENU_815   (MENU_814+1)
#define MENU_816   (MENU_815+1)
#define MENU_817   (MENU_816+1)
#define MENU_818   (MENU_817+1)
#define MENU_819   (MENU_818+1)
#define MENU_820   (MENU_819+1)
#define MENU_821   (MENU_820+1)
#define MENU_822   (MENU_821+1)
#define MENU_823   (MENU_822+1)
#define MENU_824   (MENU_823+1)
#define MENU_825   (MENU_824+1)
#define MENU_826   (MENU_825+1)
#define MENU_827   (MENU_826+1)
#define MENU_828   (MENU_827+1)
#define MENU_829   (MENU_828+1)
#define MENU_830   (MENU_829+1)
#define MENU_831   (MENU_830+1)
#define MENU_832   (MENU_831+1)
#define MENU_833   (MENU_832+1)
#define MENU_834   (MENU_833+1)
#define MENU_835   (MENU_834+1)
#define MENU_836   (MENU_835+1)
#define MENU_837   (MENU_836+1)
#define MENU_838   (MENU_837+1)
#define MENU_839   (MENU_838+1)
#define MENU_840   (MENU_839+1)
#define MENU_841   (MENU_840+1)
#define MENU_842   (MENU_841+1)
#define MENU_843   (MENU_842+1)
#define MENU_844   (MENU_843+1)
#define MENU_845   (MENU_844+1)
#define MENU_846   (MENU_845+1)
#define MENU_847   (MENU_846+1)
#define MENU_848   (MENU_847+1)
#define MENU_849   (MENU_848+1)
#define MENU_850   (MENU_849+1)
#define MENU_851   (MENU_850+1)
#define MENU_852   (MENU_851+1)
#define MENU_853   (MENU_852+1)
#define MENU_854   (MENU_853+1)
#define MENU_855   (MENU_854+1)

#define MENU_900   (MENU_855+1)  				
#define MENU_901   (MENU_900+1)
#define MENU_902   (MENU_901+1)
#define MENU_903   (MENU_902+1)  				
#define MENU_904   (MENU_903+1)   				
#define MENU_905   (MENU_904+1)
#define MENU_906   (MENU_905+1)
#define MENU_907   (MENU_906+1)
#define MENU_908   (MENU_907+1)
#define MENU_909   (MENU_908+1)
#define MENU_910   (MENU_909+1)
#define MENU_911   (MENU_910+1)
#define MENU_912   (MENU_911+1)
#define MENU_913   (MENU_912+1)
#define MENU_914   (MENU_913+1)
#define MENU_915   (MENU_914+1)
#define MENU_916   (MENU_915+1)
#define MENU_917   (MENU_916+1)
#define MENU_918   (MENU_917+1)
#define MENU_919   (MENU_918+1)
#define MENU_920   (MENU_919+1)
#define MENU_921   (MENU_920+1)
#define MENU_922   (MENU_921+1)
#define MENU_923   (MENU_922+1)
#define MENU_924   (MENU_923+1)
#define MENU_925   (MENU_924+1)
#define MENU_926   (MENU_925+1)
#define MENU_927   (MENU_926+1)
#define MENU_928   (MENU_927+1)
#define MENU_929   (MENU_928+1)
#define MENU_930   (MENU_929+1)
#define MENU_931   (MENU_930+1)
#define MENU_932   (MENU_931+1)
#define MENU_933   (MENU_932+1)
#define MENU_934   (MENU_933+1)
#define MENU_935   (MENU_934+1)
#define MENU_936   (MENU_935+1)
#define MENU_937   (MENU_936+1)
#define MENU_938   (MENU_937+1)
#define MENU_939   (MENU_938+1)
#define MENU_940   (MENU_939+1)
#define MENU_941   (MENU_940+1)
#define MENU_942   (MENU_941+1)
#define MENU_943   (MENU_942+1)
#define MENU_944   (MENU_943+1)
#define MENU_945   (MENU_944+1)
#define MENU_946   (MENU_945+1)
#define MENU_947   (MENU_946+1)
#define MENU_948   (MENU_947+1)
#define MENU_949   (MENU_948+1)
#define MENU_950   (MENU_949+1)
#define MENU_951   (MENU_950+1)
#define MENU_952   (MENU_951+1)
#define MENU_953   (MENU_952+1)
#define MENU_954   (MENU_953+1)
#define MENU_955   (MENU_954+1)

#define MENU_NUM   (MENU_955+1)
/*--------------------------------------------------------------------------------------------------*/
//菜单功能函数定义
DWORD SwitchDisp=0;
void Fun000(void) {
                    copy(&DispRam[0], "V2.0T",5);
					          copy(&DispRam[6], "CAN1:",5);
	                  
					          copy(&DispRam[72], "SP:100",6);
					
					          DispRam[11] = '0' + Can1_OK;
					          copy(&DispRam[13], "CAN2:",5);
					          DispRam[18] = '0' + Can2_OK;
	                  
	                  if (SwitchDisp==1) { 
										    DispState();
                        return;											
										}
	                  
                    DispRam[20] = '1';
                    DispRam[21] = ':';							 
                    DispRam[22] = Lift_OK[0]+'0';
                    DispRam[23] = LiftOnline[0]+'0';
					          if ((LiftState[0]&0x0800)>0) { DispRam[24] = 'O'; } //超载
					          else if ((LiftState[0]&0x1000)>0) { DispRam[24] = 'F'; } //满载
					          else if (LiftNotMoveTimer[0]>9) { DispRam[24] = 'A'; }
					          else 								            { DispRam[24] = '0' + LiftNotMoveTimer[0]; }						 
					  
                    DispRam[26] = '2';
                    DispRam[27] = ':';
                    DispRam[28] = Lift_OK[1]+'0';
                    DispRam[29] = LiftOnline[1]+'0';
					          if ((LiftState[1]&0x0800)>0) { DispRam[30] = 'O'; } //超载
					          else if ((LiftState[1]&0x1000)>0) { DispRam[30] = 'F'; } //满载
					          else if (LiftNotMoveTimer[1]>9) { DispRam[30] = 'A'; }
					          else 								            { DispRam[30] = '0' + LiftNotMoveTimer[1]; }				 

                    DispRam[32] = '3';
                    DispRam[33] = ':';
                    DispRam[34] = Lift_OK[2]+'0';
                    DispRam[35] = LiftOnline[2]+'0';
					          if ((LiftState[2]&0x0800)>0) { DispRam[36] = 'O'; } //超载
					          else if ((LiftState[2]&0x1000)>0) { DispRam[36] = 'F'; } //满载
					          else if (LiftNotMoveTimer[2]>9) { DispRam[36] = 'A'; }
					          else 								            { DispRam[36] = '0' + LiftNotMoveTimer[2]; }	 

                    DispRam[40] = '4';
                    DispRam[41] = ':';
                    DispRam[42] = Lift_OK[3]+'0';
                    DispRam[43] = LiftOnline[3]+'0';
					          if ((LiftState[3]&0x0800)>0) { DispRam[44] = 'O'; } //超载
					          else if ((LiftState[3]&0x1000)>0) { DispRam[44] = 'F'; } //满载
					          else if (LiftNotMoveTimer[3]>9) { DispRam[44] = 'A'; }
					          else 								            { DispRam[44] = '0' + LiftNotMoveTimer[3]; }	 		 

                    DispRam[46] = '5';
                    DispRam[47] = ':';
                    DispRam[48] = Lift_OK[4]+'0';
                    DispRam[49] = LiftOnline[4]+'0';
                    if ((LiftState[4]&0x0800)>0) { DispRam[50] = 'O'; } //超载
					          else if ((LiftState[4]&0x1000)>0) { DispRam[50] = 'F'; } //满载
					          else if (LiftNotMoveTimer[4]>9) { DispRam[50] = 'A'; }
					          else 								            { DispRam[50] = '0' + LiftNotMoveTimer[4]; }					 

                    DispRam[52] = '6';
                    DispRam[53] = ':';
                    DispRam[54] = Lift_OK[5]+'0';
                    DispRam[55] = LiftOnline[5]+'0';
					          if ((LiftState[5]&0x0800)>0) { DispRam[56] = 'O'; } //超载
					          else if ((LiftState[5]&0x1000)>0) { DispRam[56] = 'F'; } //满载
					          else if (LiftNotMoveTimer[5]>9) { DispRam[56] = 'A'; }
					          else 								            { DispRam[56] = '0' + LiftNotMoveTimer[5]; }

                    DispRam[60] = '7';
                    DispRam[61] = ':';
                    DispRam[62] = Lift_OK[6]+'0';			 
                    DispRam[63] = LiftOnline[6]+'0';
					          if ((LiftState[6]&0x0800)>0) { DispRam[64] = 'O'; } //超载
					          else if ((LiftState[6]&0x1000)>0) { DispRam[64] = 'F'; } //满载
					          else if (LiftNotMoveTimer[6]>9) { DispRam[64] = 'A'; }
					          else 								            { DispRam[64] = '0' + LiftNotMoveTimer[6]; }

                    DispRam[66] = '8';
                    DispRam[67] = ':';							 
                    DispRam[68] = Lift_OK[7]+'0';			 
                    DispRam[69] = LiftOnline[7]+'0';
					          if ((LiftState[7]&0x0800)>0) { DispRam[70] = 'O'; } //超载
					          else if ((LiftState[7]&0x1000)>0) { DispRam[70] = 'F'; } //满载
					          else if (LiftNotMoveTimer[7]>9) { DispRam[70] = 'A'; }
					          else 								            { DispRam[70] = '0' + LiftNotMoveTimer[7]; }				 
					//DispUpDnData(GroupUpOrder, GroupDnOrder);					 
				  } 				  
void Fun001(void) { DWORD i;				  
                    if (LiftFuntion&WaitSpecial) {
					             copy(&DispRam[0], "BaseFloor Timer:",16);
					             i = LiftWaitTimer[0];
					             DispRam[16] = i/1000+'0';  i %= 1000;
					             DispRam[17] = i/100+'0';   i %= 100;
					             DispRam[18] = i/10+'0';    i %= 10;
					             DispRam[19] = i+'0';
					          }
					          else {
					             copy(&DispRam[0], "BaseFloor:",10);
					             DispRam[11] = BaseFloorGroup+'1';
					          }
                    DispData2For3(RTC_HOUR, RTC_MIN, RTC_SEC, &DispRam[25],1);
					          DispData2For3(RTC_YEAR, RTC_MONTH, RTC_DOM, &DispRam[44],0);
					          //DispState2();										
                  }

void Fun100(void) { DispState(); } 
void Fun101(void) { FunPassword(0); }  
void Fun102(void) { FunPassword(1); }  
void Fun103(void) { MenuData2For3(&RTC_YEAR, &RTC_MONTH, &RTC_DOM, &DispRam[25], 0); }  
void Fun104(void) { MenuData2For3(&RTC_HOUR, &RTC_MIN, &RTC_SEC, &DispRam[24], 1); }  
void Fun105(void) { FunFactoryPara(); }
void Fun106(void) { MenuData(&LiftNotMove, &DispRam[29], 4|(0xff<<8)|(0xff<<16),1,9999,10); }
void Fun107(void) { MenuData(&GroupFirstFloor, &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun108(void) { MenuData(&GroupMaxFloor, &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun109(void) { GroupMenuData(WaitFloorTime, &DispRam[24], 4|(0xff<<8)|(0xff<<16),1,9999,0,5); }
void Fun110(void) { MenuSetTime(&WaitStartTime[0]); }  
void Fun111(void) { MenuSetTime(&WaitEndTime[0]);   }  
void Fun112(void) { MenuSetTime(&WaitStartTime[1]); }  
void Fun113(void) { MenuSetTime(&WaitEndTime[1]);   }
void Fun114(void) { MenuSetTime(&WaitStartTime[2]); }  
void Fun115(void) { MenuSetTime(&WaitEndTime[2]);   }  
void Fun116(void) { MenuSetTime(&WaitStartTime[3]); }  
void Fun117(void) { MenuSetTime(&WaitEndTime[3]);   }  
void Fun118(void) { MenuSetTime(&WaitStartTime[4]); }  
void Fun119(void) { MenuSetTime(&WaitEndTime[4]);   }  
void Fun120(void) { MenuSetTime(&WaitStartTime[5]); }  
void Fun121(void) { MenuSetTime(&WaitEndTime[5]); }   
void Fun122(void) { MenuCallBit(GroupUpOrder[0]); }
void Fun123(void) { MenuCallBit(GroupDnOrder[0]); }
void Fun124(void) { MenuCallBit(GroupUpOrder[1]); }
void Fun125(void) { MenuCallBit(GroupDnOrder[1]); }
void Fun126(void) { MenuCallBit(GroupUpOrder[2]); }
void Fun127(void) { MenuCallBit(GroupDnOrder[2]); }
void Fun128(void) { MenuCallBit(GroupUpOrder[3]); }
void Fun129(void) { MenuCallBit(GroupDnOrder[3]); }
void Fun130(void) { MenuCallBit(GroupUpOrder[4]); }
void Fun131(void) { MenuCallBit(GroupDnOrder[4]); }
void Fun132(void) { MenuCallBit(GroupUpOrder[5]); }
void Fun133(void) { MenuCallBit(GroupDnOrder[5]); }
void Fun134(void) { MenuCallBit(GroupUpOrder[6]); }
void Fun135(void) { MenuCallBit(GroupDnOrder[6]); }
void Fun136(void) { MenuCallBit(GroupUpOrder[7]); }
void Fun137(void) { MenuCallBit(GroupDnOrder[7]); }
void Fun138(void) { MenuBit32(&LiftFuntion); }
void Fun139(void) { GroupMenuData_Byte(WaitFloor8, &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,7); }
void Fun140(void) { GroupMenuData_Byte(WaitFloor7, &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,6); }
void Fun141(void) { GroupMenuData_Byte(WaitFloor6, &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,5); }
void Fun142(void) { GroupMenuData_Byte(WaitFloor5, &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,4); }
void Fun143(void) { GroupMenuData_Byte(WaitFloor4, &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,3); }
void Fun144(void) { GroupMenuData_Byte(WaitFloor3, &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,2); }
void Fun145(void) { GroupMenuData_Byte(WaitFloor2, &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,1); }
void Fun146(void) { GroupMenuData_Byte(PowerSetp, &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,16,1,7); }
void Fun147(void) { GroupMenuData_Byte(FireSetp, &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,16,1,7); }
void Fun148(void) { MenuData(&Mode2_WaitFloorTime, &DispRam[29], 4|(0xff<<8)|(0xff<<16),1,9999,0); }
const BYTE WeekTab[][20] = {
     {"     0.Sunday       "},
     {"     1.Monday       "},
     {"     2.Tuesday      "},
     {"     3.Wednesday    "},
     {"     4.Thursday     "},
     {"     5.Friday       "},
     {"     6.Saturday     "},
};
void Fun149(void) { MenuChar((DWORD *)(&RTC_DOW), &DispRam[20], 6, 0, WeekTab[0], 20 ); } 
void Fun150(void) { MenuData(&FloorRunTime, &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,99,0); } //电梯单层运行时间
void Fun151(void) { MenuData(&RunStopTime, &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,99,0); }  //电梯停靠时间	
void Fun152(void) { MenuData(&RunOrderTime, &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,99,0); } //电梯登记优先时间

void Fun153(void) { MenuCallBit(LiftInOrder[0]); }
void Fun154(void) { MenuCallBit(LiftInOrder[1]); }
void Fun155(void) { MenuCallBit(LiftInOrder[2]); }
void Fun156(void) { MenuCallBit(LiftInOrder[3]); }
void Fun157(void) { MenuCallBit(LiftInOrder[4]); }
void Fun158(void) { MenuCallBit(LiftInOrder[5]); }
void Fun159(void) { MenuCallBit(LiftInOrder[6]); }
void Fun160(void) { MenuCallBit(LiftInOrder[7]); }

void Fun161(void) { MenuCallBit(UpOrder[0]); }
void Fun162(void) { MenuCallBit(DnOrder[0]); }
void Fun163(void) { MenuCallBit(UpOrder[1]); }
void Fun164(void) { MenuCallBit(DnOrder[1]); }
void Fun165(void) { MenuCallBit(UpOrder[2]); }
void Fun166(void) { MenuCallBit(DnOrder[2]); }
void Fun167(void) { MenuCallBit(UpOrder[3]); }
void Fun168(void) { MenuCallBit(DnOrder[3]); }
void Fun169(void) { MenuCallBit(UpOrder[4]); }
void Fun170(void) { MenuCallBit(DnOrder[4]); }
void Fun171(void) { MenuCallBit(UpOrder[5]); }
void Fun172(void) { MenuCallBit(DnOrder[5]); }
void Fun173(void) { MenuCallBit(UpOrder[6]); }
void Fun174(void) { MenuCallBit(DnOrder[6]); }
void Fun175(void) { MenuCallBit(UpOrder[7]); }
void Fun176(void) { MenuCallBit(DnOrder[7]); }

void Fun200(void) { DispState(); }
void Fun201(void) { MenuData(&SetAddr[0], &DispRam[29], 3|(0xff<<8)|(0xff<<16),1,250,0); }
void Fun202(void) { MenuData(&FirstFloor[0], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun203(void) { MenuData(&MaxFloor[0], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun204(void) { MenuCallBitTwo(EnUpOrder[0], EnUpOrderB[0]); }
void Fun205(void) { MenuCallBitTwo(EnDnOrder[0], EnDnOrderB[0]); }
void Fun206(void) { MenuData(&NewFlag[0], &DispRam[29], 1|(0xff<<8)|(0xff<<16),1,2,0); }
void Fun207(void) { GroupMenuData_Byte(WaitFloor[0], &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,5); }
void Fun208(void) { MenuCallBit(EnUpLift[0][0]); }
void Fun209(void) { MenuCallBit(EnDnLift[0][0]); }
void Fun210(void) { MenuCallBit(EnUpLift[1][0]); }
void Fun211(void) { MenuCallBit(EnDnLift[1][0]); }
void Fun212(void) { MenuCallBit(EnUpLift[2][0]); }
void Fun213(void) { MenuCallBit(EnDnLift[2][0]); }
void Fun214(void) { MenuCallBit(EnUpLift[3][0]); }
void Fun215(void) { MenuCallBit(EnDnLift[3][0]); }
void Fun216(void) { MenuCallBit(EnUpLift[4][0]); }
void Fun217(void) { MenuCallBit(EnDnLift[4][0]); }
void Fun218(void) { MenuCallBit(EnUpLift[5][0]); }
void Fun219(void) { MenuCallBit(EnDnLift[5][0]); }
void Fun220(void) { MenuCallBit(EnUpLift[6][0]); }
void Fun221(void) { MenuCallBit(EnDnLift[6][0]); }
void Fun222(void) { MenuCallBit(EnUpLift[7][0]); }
void Fun223(void) { MenuCallBit(EnDnLift[7][0]); }
void Fun224(void) { MenuCallBit(EnUpGroup[0][0]); }
void Fun225(void) { MenuCallBit(EnDnGroup[0][0]); }
void Fun226(void) { MenuCallBit(EnUpGroup[1][0]); }
void Fun227(void) { MenuCallBit(EnDnGroup[1][0]); }
void Fun228(void) { MenuCallBit(EnUpGroup[2][0]); }
void Fun229(void) { MenuCallBit(EnDnGroup[2][0]); }
void Fun230(void) { MenuCallBit(EnUpGroup[3][0]); }
void Fun231(void) { MenuCallBit(EnDnGroup[3][0]); }
void Fun232(void) { MenuCallBit(EnUpGroup[4][0]); }
void Fun233(void) { MenuCallBit(EnDnGroup[4][0]); }
void Fun234(void) { MenuCallBit(EnUpGroup[5][0]); }
void Fun235(void) { MenuCallBit(EnDnGroup[5][0]); }
void Fun236(void) { MenuCallBit(EnUpGroup[6][0]); }
void Fun237(void) { MenuCallBit(EnDnGroup[6][0]); }
void Fun238(void) { MenuCallBit(EnUpGroup[7][0]); }
void Fun239(void) { MenuCallBit(EnDnGroup[7][0]); }
void Fun240(void) { MenuEnOrder(&OrderTimeEnable[0][0], &DispRam[22]); } 
void Fun241(void) { MenuEnOrder(&OrderTimeEnable[0][1], &DispRam[22]); } 
void Fun242(void) { MenuEnOrder(&OrderTimeEnable[0][2], &DispRam[22]); } 
void Fun243(void) { MenuEnOrder(&OrderTimeEnable[0][3], &DispRam[22]); } 
void Fun244(void) { MenuEnOrder(&OrderTimeEnable[0][4], &DispRam[22]); } 
void Fun245(void) { MenuEnOrder(&OrderTimeEnable[0][5], &DispRam[22]); } 
void Fun246(void) { MenuEnOrder(&OrderTimeEnable[0][6], &DispRam[22]); } 
void Fun247(void) { MenuEnOrder(&OrderTimeEnable[0][7], &DispRam[22]); } 
void Fun248(void) { MenuCallBit(OrderFlagEnable[0][0]); } 
void Fun249(void) { MenuCallBit(OrderFlagEnable[0][1]); }
void Fun250(void) { MenuCallBit(OrderFlagEnable[0][2]); }
void Fun251(void) { MenuCallBit(OrderFlagEnable[0][3]); }
void Fun252(void) { MenuCallBit(OrderFlagEnable[0][4]); }
void Fun253(void) { MenuCallBit(OrderFlagEnable[0][5]); }
void Fun254(void) { MenuCallBit(OrderFlagEnable[0][6]); }
void Fun255(void) { MenuCallBit(OrderFlagEnable[0][7]); }

void Fun300(void) { DispState(); }
void Fun301(void) { MenuData(&SetAddr[1], &DispRam[29], 3|(0xff<<8)|(0xff<<16),1,250,0); }
void Fun302(void) { MenuData(&FirstFloor[1], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun303(void) { MenuData(&MaxFloor[1], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun304(void) { MenuCallBitTwo(EnUpOrder[1],EnUpOrderB[1]); }
void Fun305(void) { MenuCallBitTwo(EnDnOrder[1],EnDnOrderB[1]); }
void Fun306(void) { MenuData(&NewFlag[1], &DispRam[29], 1|(0xff<<8)|(0xff<<16),1,2,0); }
void Fun307(void) { GroupMenuData_Byte(WaitFloor[1], &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,5); }
void Fun308(void) { MenuCallBit(EnUpLift[0][1]); }
void Fun309(void) { MenuCallBit(EnDnLift[0][1]); }
void Fun310(void) { MenuCallBit(EnUpLift[1][1]); }
void Fun311(void) { MenuCallBit(EnDnLift[1][1]); }
void Fun312(void) { MenuCallBit(EnUpLift[2][1]); }
void Fun313(void) { MenuCallBit(EnDnLift[2][1]); }
void Fun314(void) { MenuCallBit(EnUpLift[3][1]); }
void Fun315(void) { MenuCallBit(EnDnLift[3][1]); }
void Fun316(void) { MenuCallBit(EnUpLift[4][1]); }
void Fun317(void) { MenuCallBit(EnDnLift[4][1]); }
void Fun318(void) { MenuCallBit(EnUpLift[5][1]); }
void Fun319(void) { MenuCallBit(EnDnLift[5][1]); }
void Fun320(void) { MenuCallBit(EnUpLift[6][1]); }
void Fun321(void) { MenuCallBit(EnDnLift[6][1]); }
void Fun322(void) { MenuCallBit(EnUpLift[7][1]); }
void Fun323(void) { MenuCallBit(EnDnLift[7][1]); }
void Fun324(void) { MenuCallBit(EnUpGroup[0][1]); }
void Fun325(void) { MenuCallBit(EnDnGroup[0][1]); }
void Fun326(void) { MenuCallBit(EnUpGroup[1][1]); }
void Fun327(void) { MenuCallBit(EnDnGroup[1][1]); }
void Fun328(void) { MenuCallBit(EnUpGroup[2][1]); }
void Fun329(void) { MenuCallBit(EnDnGroup[2][1]); }
void Fun330(void) { MenuCallBit(EnUpGroup[3][1]); }
void Fun331(void) { MenuCallBit(EnDnGroup[3][1]); }
void Fun332(void) { MenuCallBit(EnUpGroup[4][1]); }
void Fun333(void) { MenuCallBit(EnDnGroup[4][1]); }
void Fun334(void) { MenuCallBit(EnUpGroup[5][1]); }
void Fun335(void) { MenuCallBit(EnDnGroup[5][1]); }
void Fun336(void) { MenuCallBit(EnUpGroup[6][1]); }
void Fun337(void) { MenuCallBit(EnDnGroup[6][1]); }
void Fun338(void) { MenuCallBit(EnUpGroup[7][1]); }
void Fun339(void) { MenuCallBit(EnDnGroup[7][1]); }
void Fun340(void) { MenuEnOrder(&OrderTimeEnable[1][0], &DispRam[22]); } 
void Fun341(void) { MenuEnOrder(&OrderTimeEnable[1][1], &DispRam[22]); } 
void Fun342(void) { MenuEnOrder(&OrderTimeEnable[1][2], &DispRam[22]); } 
void Fun343(void) { MenuEnOrder(&OrderTimeEnable[1][3], &DispRam[22]); } 
void Fun344(void) { MenuEnOrder(&OrderTimeEnable[1][4], &DispRam[22]); } 
void Fun345(void) { MenuEnOrder(&OrderTimeEnable[1][5], &DispRam[22]); } 
void Fun346(void) { MenuEnOrder(&OrderTimeEnable[1][6], &DispRam[22]); } 
void Fun347(void) { MenuEnOrder(&OrderTimeEnable[1][7], &DispRam[22]); } 
void Fun348(void) { MenuCallBit(OrderFlagEnable[1][0]); }
void Fun349(void) { MenuCallBit(OrderFlagEnable[1][1]); }
void Fun350(void) { MenuCallBit(OrderFlagEnable[1][2]); }
void Fun351(void) { MenuCallBit(OrderFlagEnable[1][3]); }
void Fun352(void) { MenuCallBit(OrderFlagEnable[1][4]); }
void Fun353(void) { MenuCallBit(OrderFlagEnable[1][5]); }
void Fun354(void) { MenuCallBit(OrderFlagEnable[1][6]); }
void Fun355(void) { MenuCallBit(OrderFlagEnable[1][7]); }

void Fun400(void) { DispState(); }
void Fun401(void) { MenuData(&SetAddr[2], &DispRam[29], 3|(0xff<<8)|(0xff<<16),1,250,0); }
void Fun402(void) { MenuData(&FirstFloor[2], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun403(void) { MenuData(&MaxFloor[2], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun404(void) { MenuCallBitTwo(EnUpOrder[2],EnUpOrderB[2]); }
void Fun405(void) { MenuCallBitTwo(EnDnOrder[2],EnDnOrderB[2]); }
void Fun406(void) { MenuData(&NewFlag[2], &DispRam[29], 1|(0xff<<8)|(0xff<<16),1,2,0); }
void Fun407(void) { GroupMenuData_Byte(WaitFloor[2], &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,5); }
void Fun408(void) { MenuCallBit(EnUpLift[0][2]); }
void Fun409(void) { MenuCallBit(EnDnLift[0][2]); }
void Fun410(void) { MenuCallBit(EnUpLift[1][2]); }
void Fun411(void) { MenuCallBit(EnDnLift[1][2]); }
void Fun412(void) { MenuCallBit(EnUpLift[2][2]); }
void Fun413(void) { MenuCallBit(EnDnLift[2][2]); }
void Fun414(void) { MenuCallBit(EnUpLift[3][2]); }
void Fun415(void) { MenuCallBit(EnDnLift[3][2]); }
void Fun416(void) { MenuCallBit(EnUpLift[4][2]); }
void Fun417(void) { MenuCallBit(EnDnLift[4][2]); }
void Fun418(void) { MenuCallBit(EnUpLift[5][2]); }
void Fun419(void) { MenuCallBit(EnDnLift[5][2]); }
void Fun420(void) { MenuCallBit(EnUpLift[6][2]); }
void Fun421(void) { MenuCallBit(EnDnLift[6][2]); }
void Fun422(void) { MenuCallBit(EnUpLift[7][2]); }
void Fun423(void) { MenuCallBit(EnDnLift[7][2]); }
void Fun424(void) { MenuCallBit(EnUpGroup[0][2]); }
void Fun425(void) { MenuCallBit(EnDnGroup[0][2]); }
void Fun426(void) { MenuCallBit(EnUpGroup[1][2]); }
void Fun427(void) { MenuCallBit(EnDnGroup[1][2]); }
void Fun428(void) { MenuCallBit(EnUpGroup[2][2]); }
void Fun429(void) { MenuCallBit(EnDnGroup[2][2]); }
void Fun430(void) { MenuCallBit(EnUpGroup[3][2]); }
void Fun431(void) { MenuCallBit(EnDnGroup[3][2]); }
void Fun432(void) { MenuCallBit(EnUpGroup[4][2]); }
void Fun433(void) { MenuCallBit(EnDnGroup[4][2]); }
void Fun434(void) { MenuCallBit(EnUpGroup[5][2]); }
void Fun435(void) { MenuCallBit(EnDnGroup[5][2]); }
void Fun436(void) { MenuCallBit(EnUpGroup[6][2]); }
void Fun437(void) { MenuCallBit(EnDnGroup[6][2]); }
void Fun438(void) { MenuCallBit(EnUpGroup[7][2]); }
void Fun439(void) { MenuCallBit(EnDnGroup[7][2]); }
void Fun440(void) { MenuEnOrder(&OrderTimeEnable[2][0], &DispRam[22]); } 
void Fun441(void) { MenuEnOrder(&OrderTimeEnable[2][1], &DispRam[22]); } 
void Fun442(void) { MenuEnOrder(&OrderTimeEnable[2][2], &DispRam[22]); } 
void Fun443(void) { MenuEnOrder(&OrderTimeEnable[2][3], &DispRam[22]); } 
void Fun444(void) { MenuEnOrder(&OrderTimeEnable[2][4], &DispRam[22]); } 
void Fun445(void) { MenuEnOrder(&OrderTimeEnable[2][5], &DispRam[22]); } 
void Fun446(void) { MenuEnOrder(&OrderTimeEnable[2][6], &DispRam[22]); } 
void Fun447(void) { MenuEnOrder(&OrderTimeEnable[2][7], &DispRam[22]); } 
void Fun448(void) { MenuCallBit(OrderFlagEnable[2][0]); }
void Fun449(void) { MenuCallBit(OrderFlagEnable[2][1]); }
void Fun450(void) { MenuCallBit(OrderFlagEnable[2][2]); }
void Fun451(void) { MenuCallBit(OrderFlagEnable[2][3]); }
void Fun452(void) { MenuCallBit(OrderFlagEnable[2][4]); }
void Fun453(void) { MenuCallBit(OrderFlagEnable[2][5]); }
void Fun454(void) { MenuCallBit(OrderFlagEnable[2][6]); }
void Fun455(void) { MenuCallBit(OrderFlagEnable[2][7]); }

void Fun500(void) { DispState(); }
void Fun501(void) { MenuData(&SetAddr[3], &DispRam[29], 3|(0xff<<8)|(0xff<<16),1,250,0); }
void Fun502(void) { MenuData(&FirstFloor[3], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun503(void) { MenuData(&MaxFloor[3], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun504(void) { MenuCallBitTwo(EnUpOrder[3],EnUpOrderB[3]); }
void Fun505(void) { MenuCallBitTwo(EnDnOrder[3],EnDnOrderB[3]); }
void Fun506(void) { MenuData(&NewFlag[3], &DispRam[29], 1|(0xff<<8)|(0xff<<16),1,2,0); }
void Fun507(void) { GroupMenuData_Byte(WaitFloor[3], &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,5); }
void Fun508(void) { MenuCallBit(EnUpLift[0][3]); }
void Fun509(void) { MenuCallBit(EnDnLift[0][3]); }
void Fun510(void) { MenuCallBit(EnUpLift[1][3]); }
void Fun511(void) { MenuCallBit(EnDnLift[1][3]); }
void Fun512(void) { MenuCallBit(EnUpLift[2][3]); }
void Fun513(void) { MenuCallBit(EnDnLift[2][3]); }
void Fun514(void) { MenuCallBit(EnUpLift[3][3]); }
void Fun515(void) { MenuCallBit(EnDnLift[3][3]); }
void Fun516(void) { MenuCallBit(EnUpLift[4][3]); }
void Fun517(void) { MenuCallBit(EnDnLift[4][3]); }
void Fun518(void) { MenuCallBit(EnUpLift[5][3]); }
void Fun519(void) { MenuCallBit(EnDnLift[5][3]); }
void Fun520(void) { MenuCallBit(EnUpLift[6][3]); }
void Fun521(void) { MenuCallBit(EnDnLift[6][3]); }
void Fun522(void) { MenuCallBit(EnUpLift[7][3]); }
void Fun523(void) { MenuCallBit(EnDnLift[7][3]); }
void Fun524(void) { MenuCallBit(EnUpGroup[0][3]); }
void Fun525(void) { MenuCallBit(EnDnGroup[0][3]); }
void Fun526(void) { MenuCallBit(EnUpGroup[1][3]); }
void Fun527(void) { MenuCallBit(EnDnGroup[1][3]); }
void Fun528(void) { MenuCallBit(EnUpGroup[2][3]); }
void Fun529(void) { MenuCallBit(EnDnGroup[2][3]); }
void Fun530(void) { MenuCallBit(EnUpGroup[3][3]); }
void Fun531(void) { MenuCallBit(EnDnGroup[3][3]); }
void Fun532(void) { MenuCallBit(EnUpGroup[4][3]); }
void Fun533(void) { MenuCallBit(EnDnGroup[4][3]); }
void Fun534(void) { MenuCallBit(EnUpGroup[5][3]); }
void Fun535(void) { MenuCallBit(EnDnGroup[5][3]); }
void Fun536(void) { MenuCallBit(EnUpGroup[6][3]); }
void Fun537(void) { MenuCallBit(EnDnGroup[6][3]); }
void Fun538(void) { MenuCallBit(EnUpGroup[7][3]); }
void Fun539(void) { MenuCallBit(EnDnGroup[7][3]); }
void Fun540(void) { MenuEnOrder(&OrderTimeEnable[3][0], &DispRam[22]); } 
void Fun541(void) { MenuEnOrder(&OrderTimeEnable[3][1], &DispRam[22]); } 
void Fun542(void) { MenuEnOrder(&OrderTimeEnable[3][2], &DispRam[22]); } 
void Fun543(void) { MenuEnOrder(&OrderTimeEnable[3][3], &DispRam[22]); } 
void Fun544(void) { MenuEnOrder(&OrderTimeEnable[3][4], &DispRam[22]); } 
void Fun545(void) { MenuEnOrder(&OrderTimeEnable[3][5], &DispRam[22]); } 
void Fun546(void) { MenuEnOrder(&OrderTimeEnable[3][6], &DispRam[22]); } 
void Fun547(void) { MenuEnOrder(&OrderTimeEnable[3][7], &DispRam[22]); } 
void Fun548(void) { MenuCallBit(OrderFlagEnable[3][0]); }
void Fun549(void) { MenuCallBit(OrderFlagEnable[3][1]); }
void Fun550(void) { MenuCallBit(OrderFlagEnable[3][2]); }
void Fun551(void) { MenuCallBit(OrderFlagEnable[3][3]); }
void Fun552(void) { MenuCallBit(OrderFlagEnable[3][4]); }
void Fun553(void) { MenuCallBit(OrderFlagEnable[3][5]); }
void Fun554(void) { MenuCallBit(OrderFlagEnable[3][6]); }
void Fun555(void) { MenuCallBit(OrderFlagEnable[3][7]); }

void Fun600(void) { DispState(); }
void Fun601(void) { MenuData(&SetAddr[4], &DispRam[29], 3|(0xff<<8)|(0xff<<16),1,250,0); }
void Fun602(void) { MenuData(&FirstFloor[4], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun603(void) { MenuData(&MaxFloor[4], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun604(void) { MenuCallBitTwo(EnUpOrder[4],EnUpOrderB[4]); }
void Fun605(void) { MenuCallBitTwo(EnDnOrder[4],EnDnOrderB[4]); }
void Fun606(void) { MenuData(&NewFlag[4], &DispRam[29], 1|(0xff<<8)|(0xff<<16),1,2,0); }
void Fun607(void) { GroupMenuData_Byte(WaitFloor[4], &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,5); }
void Fun608(void) { MenuCallBit(EnUpLift[0][4]); }
void Fun609(void) { MenuCallBit(EnDnLift[0][4]); }
void Fun610(void) { MenuCallBit(EnUpLift[1][4]); }
void Fun611(void) { MenuCallBit(EnDnLift[1][4]); }
void Fun612(void) { MenuCallBit(EnUpLift[2][4]); }
void Fun613(void) { MenuCallBit(EnDnLift[2][4]); }
void Fun614(void) { MenuCallBit(EnUpLift[3][4]); }
void Fun615(void) { MenuCallBit(EnDnLift[3][4]); }
void Fun616(void) { MenuCallBit(EnUpLift[4][4]); }
void Fun617(void) { MenuCallBit(EnDnLift[4][4]); }
void Fun618(void) { MenuCallBit(EnUpLift[5][4]); }
void Fun619(void) { MenuCallBit(EnDnLift[5][4]); }
void Fun620(void) { MenuCallBit(EnUpLift[6][4]); }
void Fun621(void) { MenuCallBit(EnDnLift[6][4]); }
void Fun622(void) { MenuCallBit(EnUpLift[7][4]); }
void Fun623(void) { MenuCallBit(EnDnLift[7][4]); }
void Fun624(void) { MenuCallBit(EnUpGroup[0][4]); }
void Fun625(void) { MenuCallBit(EnDnGroup[0][4]); }
void Fun626(void) { MenuCallBit(EnUpGroup[1][4]); }
void Fun627(void) { MenuCallBit(EnDnGroup[1][4]); }
void Fun628(void) { MenuCallBit(EnUpGroup[2][4]); }
void Fun629(void) { MenuCallBit(EnDnGroup[2][4]); }
void Fun630(void) { MenuCallBit(EnUpGroup[3][4]); }
void Fun631(void) { MenuCallBit(EnDnGroup[3][4]); }
void Fun632(void) { MenuCallBit(EnUpGroup[4][4]); }
void Fun633(void) { MenuCallBit(EnDnGroup[4][4]); }
void Fun634(void) { MenuCallBit(EnUpGroup[5][4]); }
void Fun635(void) { MenuCallBit(EnDnGroup[5][4]); }
void Fun636(void) { MenuCallBit(EnUpGroup[6][4]); }
void Fun637(void) { MenuCallBit(EnDnGroup[6][4]); }
void Fun638(void) { MenuCallBit(EnUpGroup[7][4]); }
void Fun639(void) { MenuCallBit(EnDnGroup[7][4]); }
void Fun640(void) { MenuEnOrder(&OrderTimeEnable[4][0], &DispRam[22]); } 
void Fun641(void) { MenuEnOrder(&OrderTimeEnable[4][1], &DispRam[22]); } 
void Fun642(void) { MenuEnOrder(&OrderTimeEnable[4][2], &DispRam[22]); } 
void Fun643(void) { MenuEnOrder(&OrderTimeEnable[4][3], &DispRam[22]); } 
void Fun644(void) { MenuEnOrder(&OrderTimeEnable[4][4], &DispRam[22]); } 
void Fun645(void) { MenuEnOrder(&OrderTimeEnable[4][5], &DispRam[22]); } 
void Fun646(void) { MenuEnOrder(&OrderTimeEnable[4][6], &DispRam[22]); } 
void Fun647(void) { MenuEnOrder(&OrderTimeEnable[4][7], &DispRam[22]); } 
void Fun648(void) { MenuCallBit(OrderFlagEnable[4][0]); }
void Fun649(void) { MenuCallBit(OrderFlagEnable[4][1]); }
void Fun650(void) { MenuCallBit(OrderFlagEnable[4][2]); }
void Fun651(void) { MenuCallBit(OrderFlagEnable[4][3]); }
void Fun652(void) { MenuCallBit(OrderFlagEnable[4][4]); }
void Fun653(void) { MenuCallBit(OrderFlagEnable[4][5]); }
void Fun654(void) { MenuCallBit(OrderFlagEnable[4][6]); }
void Fun655(void) { MenuCallBit(OrderFlagEnable[4][7]); }

void Fun700(void) { DispState(); }
void Fun701(void) { MenuData(&SetAddr[5], &DispRam[29], 3|(0xff<<8)|(0xff<<16),1,250,0); }
void Fun702(void) { MenuData(&FirstFloor[5], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun703(void) { MenuData(&MaxFloor[5], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun704(void) { MenuCallBitTwo(EnUpOrder[5],EnUpOrderB[5]); }
void Fun705(void) { MenuCallBitTwo(EnDnOrder[5],EnDnOrderB[5]); }
void Fun706(void) { MenuData(&NewFlag[5], &DispRam[29], 1|(0xff<<8)|(0xff<<16),1,2,0); }
void Fun707(void) { GroupMenuData_Byte(WaitFloor[5], &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,5); }
void Fun708(void) { MenuCallBit(EnUpLift[0][5]); }
void Fun709(void) { MenuCallBit(EnDnLift[0][5]); }
void Fun710(void) { MenuCallBit(EnUpLift[1][5]); }
void Fun711(void) { MenuCallBit(EnDnLift[1][5]); }
void Fun712(void) { MenuCallBit(EnUpLift[2][5]); }
void Fun713(void) { MenuCallBit(EnDnLift[2][5]); }
void Fun714(void) { MenuCallBit(EnUpLift[3][5]); }
void Fun715(void) { MenuCallBit(EnDnLift[3][5]); }
void Fun716(void) { MenuCallBit(EnUpLift[4][5]); }
void Fun717(void) { MenuCallBit(EnDnLift[4][5]); }
void Fun718(void) { MenuCallBit(EnUpLift[5][5]); }
void Fun719(void) { MenuCallBit(EnDnLift[5][5]); }
void Fun720(void) { MenuCallBit(EnUpLift[6][5]); }
void Fun721(void) { MenuCallBit(EnDnLift[6][5]); }
void Fun722(void) { MenuCallBit(EnUpLift[7][5]); }
void Fun723(void) { MenuCallBit(EnDnLift[7][5]); }
void Fun724(void) { MenuCallBit(EnUpGroup[0][5]); }
void Fun725(void) { MenuCallBit(EnDnGroup[0][5]); }
void Fun726(void) { MenuCallBit(EnUpGroup[1][5]); }
void Fun727(void) { MenuCallBit(EnDnGroup[1][5]); }
void Fun728(void) { MenuCallBit(EnUpGroup[2][5]); }
void Fun729(void) { MenuCallBit(EnDnGroup[2][5]); }
void Fun730(void) { MenuCallBit(EnUpGroup[3][5]); }
void Fun731(void) { MenuCallBit(EnDnGroup[3][5]); }
void Fun732(void) { MenuCallBit(EnUpGroup[4][5]); }
void Fun733(void) { MenuCallBit(EnDnGroup[4][5]); }
void Fun734(void) { MenuCallBit(EnUpGroup[5][5]); }
void Fun735(void) { MenuCallBit(EnDnGroup[5][5]); }
void Fun736(void) { MenuCallBit(EnUpGroup[6][5]); }
void Fun737(void) { MenuCallBit(EnDnGroup[6][5]); }
void Fun738(void) { MenuCallBit(EnUpGroup[7][5]); }
void Fun739(void) { MenuCallBit(EnDnGroup[7][5]); }
void Fun740(void) { MenuEnOrder(&OrderTimeEnable[5][0], &DispRam[22]); } 
void Fun741(void) { MenuEnOrder(&OrderTimeEnable[5][1], &DispRam[22]); } 
void Fun742(void) { MenuEnOrder(&OrderTimeEnable[5][2], &DispRam[22]); } 
void Fun743(void) { MenuEnOrder(&OrderTimeEnable[5][3], &DispRam[22]); } 
void Fun744(void) { MenuEnOrder(&OrderTimeEnable[5][4], &DispRam[22]); } 
void Fun745(void) { MenuEnOrder(&OrderTimeEnable[5][5], &DispRam[22]); } 
void Fun746(void) { MenuEnOrder(&OrderTimeEnable[5][6], &DispRam[22]); } 
void Fun747(void) { MenuEnOrder(&OrderTimeEnable[5][7], &DispRam[22]); } 
void Fun748(void) { MenuCallBit(OrderFlagEnable[5][0]); }
void Fun749(void) { MenuCallBit(OrderFlagEnable[5][1]); }
void Fun750(void) { MenuCallBit(OrderFlagEnable[5][2]); }
void Fun751(void) { MenuCallBit(OrderFlagEnable[5][3]); }
void Fun752(void) { MenuCallBit(OrderFlagEnable[5][4]); }
void Fun753(void) { MenuCallBit(OrderFlagEnable[5][5]); }
void Fun754(void) { MenuCallBit(OrderFlagEnable[5][6]); }
void Fun755(void) { MenuCallBit(OrderFlagEnable[5][7]); }

void Fun800(void) { DispState(); }
void Fun801(void) { MenuData(&SetAddr[6], &DispRam[29], 3|(0xff<<8)|(0xff<<16),1,250,0); }
void Fun802(void) { MenuData(&FirstFloor[6], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun803(void) { MenuData(&MaxFloor[6], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun804(void) { MenuCallBitTwo(EnUpOrder[6],EnUpOrderB[6]); }
void Fun805(void) { MenuCallBitTwo(EnDnOrder[6],EnDnOrderB[6]); }
void Fun806(void) { MenuData(&NewFlag[6], &DispRam[29], 1|(0xff<<8)|(0xff<<16),1,2,0); }
void Fun807(void) { GroupMenuData_Byte(WaitFloor[6], &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,5); }
void Fun808(void) { MenuCallBit(EnUpLift[0][6]); }
void Fun809(void) { MenuCallBit(EnDnLift[0][6]); }
void Fun810(void) { MenuCallBit(EnUpLift[1][6]); }
void Fun811(void) { MenuCallBit(EnDnLift[1][6]); }
void Fun812(void) { MenuCallBit(EnUpLift[2][6]); }
void Fun813(void) { MenuCallBit(EnDnLift[2][6]); }
void Fun814(void) { MenuCallBit(EnUpLift[3][6]); }
void Fun815(void) { MenuCallBit(EnDnLift[3][6]); }
void Fun816(void) { MenuCallBit(EnUpLift[4][6]); }
void Fun817(void) { MenuCallBit(EnDnLift[4][6]); }
void Fun818(void) { MenuCallBit(EnUpLift[5][6]); }
void Fun819(void) { MenuCallBit(EnDnLift[5][6]); }
void Fun820(void) { MenuCallBit(EnUpLift[6][6]); }
void Fun821(void) { MenuCallBit(EnDnLift[6][6]); }
void Fun822(void) { MenuCallBit(EnUpLift[7][6]); }
void Fun823(void) { MenuCallBit(EnDnLift[7][6]); }
void Fun824(void) { MenuCallBit(EnUpGroup[0][6]); }
void Fun825(void) { MenuCallBit(EnDnGroup[0][6]); }
void Fun826(void) { MenuCallBit(EnUpGroup[1][6]); }
void Fun827(void) { MenuCallBit(EnDnGroup[1][6]); }
void Fun828(void) { MenuCallBit(EnUpGroup[2][6]); }
void Fun829(void) { MenuCallBit(EnDnGroup[2][6]); }
void Fun830(void) { MenuCallBit(EnUpGroup[3][6]); }
void Fun831(void) { MenuCallBit(EnDnGroup[3][6]); }
void Fun832(void) { MenuCallBit(EnUpGroup[4][6]); }
void Fun833(void) { MenuCallBit(EnDnGroup[4][6]); }
void Fun834(void) { MenuCallBit(EnUpGroup[5][6]); }
void Fun835(void) { MenuCallBit(EnDnGroup[5][6]); }
void Fun836(void) { MenuCallBit(EnUpGroup[6][6]); }
void Fun837(void) { MenuCallBit(EnDnGroup[6][6]); }
void Fun838(void) { MenuCallBit(EnUpGroup[7][6]); }
void Fun839(void) { MenuCallBit(EnDnGroup[7][6]); }
void Fun840(void) { MenuEnOrder(&OrderTimeEnable[6][0], &DispRam[22]); } 
void Fun841(void) { MenuEnOrder(&OrderTimeEnable[6][1], &DispRam[22]); } 
void Fun842(void) { MenuEnOrder(&OrderTimeEnable[6][2], &DispRam[22]); } 
void Fun843(void) { MenuEnOrder(&OrderTimeEnable[6][3], &DispRam[22]); } 
void Fun844(void) { MenuEnOrder(&OrderTimeEnable[6][4], &DispRam[22]); } 
void Fun845(void) { MenuEnOrder(&OrderTimeEnable[6][5], &DispRam[22]); } 
void Fun846(void) { MenuEnOrder(&OrderTimeEnable[6][6], &DispRam[22]); } 
void Fun847(void) { MenuEnOrder(&OrderTimeEnable[6][7], &DispRam[22]); } 
void Fun848(void) { MenuCallBit(OrderFlagEnable[6][0]); }
void Fun849(void) { MenuCallBit(OrderFlagEnable[6][1]); }
void Fun850(void) { MenuCallBit(OrderFlagEnable[6][2]); }
void Fun851(void) { MenuCallBit(OrderFlagEnable[6][3]); }
void Fun852(void) { MenuCallBit(OrderFlagEnable[6][4]); }
void Fun853(void) { MenuCallBit(OrderFlagEnable[6][5]); }
void Fun854(void) { MenuCallBit(OrderFlagEnable[6][6]); }
void Fun855(void) { MenuCallBit(OrderFlagEnable[6][7]); }

void Fun900(void) { DispState(); }
void Fun901(void) { MenuData(&SetAddr[7], &DispRam[29], 3|(0xff<<8)|(0xff<<16),1,250,0); }
void Fun902(void) { MenuData(&FirstFloor[7], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun903(void) { MenuData(&MaxFloor[7], &DispRam[29], 2|(0xff<<8)|(0xff<<16),1,64,0); }
void Fun904(void) { MenuCallBitTwo(EnUpOrder[7], EnUpOrderB[7]); }
void Fun905(void) { MenuCallBitTwo(EnDnOrder[7], EnDnOrderB[7]); }
void Fun906(void) { MenuData(&NewFlag[7], &DispRam[29], 1|(0xff<<8)|(0xff<<16),1,2,0); }
void Fun907(void) { GroupMenuData_Byte(WaitFloor[7], &DispRam[24], 2|(0xff<<8)|(0xff<<16),1,64,0,5); }
void Fun908(void) { MenuCallBit(EnUpLift[0][7]);  }
void Fun909(void) { MenuCallBit(EnDnLift[0][7]);  }
void Fun910(void) { MenuCallBit(EnUpLift[1][7]);  }
void Fun911(void) { MenuCallBit(EnDnLift[1][7]);  }
void Fun912(void) { MenuCallBit(EnUpLift[2][7]);  }
void Fun913(void) { MenuCallBit(EnDnLift[2][7]);  }
void Fun914(void) { MenuCallBit(EnUpLift[3][7]);  }
void Fun915(void) { MenuCallBit(EnDnLift[3][7]);  }
void Fun916(void) { MenuCallBit(EnUpLift[4][7]);  }
void Fun917(void) { MenuCallBit(EnDnLift[4][7]);  }
void Fun918(void) { MenuCallBit(EnUpLift[5][7]);  }
void Fun919(void) { MenuCallBit(EnDnLift[5][7]);  }
void Fun920(void) { MenuCallBit(EnUpLift[6][7]);  }
void Fun921(void) { MenuCallBit(EnDnLift[6][7]);  }
void Fun922(void) { MenuCallBit(EnUpLift[7][7]);  }
void Fun923(void) { MenuCallBit(EnDnLift[7][7]);  }
void Fun924(void) { MenuCallBit(EnUpGroup[0][7]); }
void Fun925(void) { MenuCallBit(EnDnGroup[0][7]); }
void Fun926(void) { MenuCallBit(EnUpGroup[1][7]); }
void Fun927(void) { MenuCallBit(EnDnGroup[1][7]); }
void Fun928(void) { MenuCallBit(EnUpGroup[2][7]); }
void Fun929(void) { MenuCallBit(EnDnGroup[2][7]); }
void Fun930(void) { MenuCallBit(EnUpGroup[3][7]); }
void Fun931(void) { MenuCallBit(EnDnGroup[3][7]); }
void Fun932(void) { MenuCallBit(EnUpGroup[4][7]); }
void Fun933(void) { MenuCallBit(EnDnGroup[4][7]); }
void Fun934(void) { MenuCallBit(EnUpGroup[5][7]); }
void Fun935(void) { MenuCallBit(EnDnGroup[5][7]); }
void Fun936(void) { MenuCallBit(EnUpGroup[6][7]); }
void Fun937(void) { MenuCallBit(EnDnGroup[6][7]); }
void Fun938(void) { MenuCallBit(EnUpGroup[7][7]); }
void Fun939(void) { MenuCallBit(EnDnGroup[7][7]); }
void Fun940(void) { MenuEnOrder(&OrderTimeEnable[7][0], &DispRam[22]); } 
void Fun941(void) { MenuEnOrder(&OrderTimeEnable[7][1], &DispRam[22]); } 
void Fun942(void) { MenuEnOrder(&OrderTimeEnable[7][2], &DispRam[22]); } 
void Fun943(void) { MenuEnOrder(&OrderTimeEnable[7][3], &DispRam[22]); } 
void Fun944(void) { MenuEnOrder(&OrderTimeEnable[7][4], &DispRam[22]); } 
void Fun945(void) { MenuEnOrder(&OrderTimeEnable[7][5], &DispRam[22]); } 
void Fun946(void) { MenuEnOrder(&OrderTimeEnable[7][6], &DispRam[22]); } 
void Fun947(void) { MenuEnOrder(&OrderTimeEnable[7][7], &DispRam[22]); } 
void Fun948(void) { MenuCallBit(OrderFlagEnable[7][0]); }
void Fun949(void) { MenuCallBit(OrderFlagEnable[7][1]); }
void Fun950(void) { MenuCallBit(OrderFlagEnable[7][2]); }
void Fun951(void) { MenuCallBit(OrderFlagEnable[7][3]); }
void Fun952(void) { MenuCallBit(OrderFlagEnable[7][4]); }
void Fun953(void) { MenuCallBit(OrderFlagEnable[7][5]); }
void Fun954(void) { MenuCallBit(OrderFlagEnable[7][6]); }
void Fun955(void) { MenuCallBit(OrderFlagEnable[7][7]); }

/*--------------------------------------------------------------------------------------------------*/
#define En_CharLong  20
//#define Cn_CharLong	 20
typedef struct
{
    BYTE ETab[En_CharLong+1];
//	BYTE CTab[Cn_CharLong+1];
	DWORD OkMenuID;		
    DWORD EscMenuID;    
    DWORD UpMenuID;
    DWORD DownMenuID;	
    DWORD ModifyEn;
    void (*CurrentOperate)();
} KeyTabStruct;
void  (*KeyFuncPtr)(); //按键功能指针

const KeyTabStruct KeyTab[MENU_NUM] =
{   
 //     12345678901234567890,     12345678901234567890	   OkMenuID, EscMenuID, DownMenuID, UpMenuID, ModifyEn (*CurOperate)()
     {{"                    "},/* {"                    "},*/MENU_001, MENU_000, MENU_000, MENU_000,  0x00 , *Fun000}, 
     {{"                    "},/* {"                    "},*/MENU_100, MENU_000, MENU_001, MENU_001,  0x00 , *Fun001},
	  
     {{"100 System          "},/* {"100.系统参数        "},*/MENU_101, MENU_001, MENU_200, MENU_100,  0x00 , *Fun100},
     {{"101 Password 1      "},/* {"101.密码1           "},*/MENU_101, MENU_100, MENU_102, MENU_101,  0x01 , *Fun101},
     {{"102 Password 2      "},/* {"102.密码2           "},*/MENU_102, MENU_100, MENU_103, MENU_101,  0x01 , *Fun102},
     {{"103 System Date     "},/* {"103.系统日期        "},*/MENU_103, MENU_100, MENU_104, MENU_102,  0x03 , *Fun103},
	   {{"104 System Time     "},/* {"104.系统时间        "},*/MENU_104, MENU_100, MENU_105, MENU_103,  0x03 , *Fun104},
     {{"105 Factory         "},/* {"105.出厂设置        "},*/MENU_105, MENU_100, MENU_106, MENU_104,  0x03 , *Fun105},
     {{"106 Stop OverTime   "},/* {"106.停车超时        "},*/MENU_106, MENU_100, MENU_107, MENU_105,  0x03 , *Fun106},
     {{"107 First Floor     "},/* {"305.首层地址        "},*/MENU_107, MENU_100, MENU_108, MENU_106,  0x03 , *Fun107}, 
     {{"108 Max Floor       "},/* {"305.最高层地址      "},*/MENU_108, MENU_100, MENU_109, MENU_107,  0x03 , *Fun108}, 
     {{"109 BaseFloor Time  "},/* {"304.回基站时间      "},*/MENU_109, MENU_100, MENU_110, MENU_108,  0x01 , *Fun109}, 
     {{"110 Start Time 1    "},/* {"304.回基站时间      "},*/MENU_110, MENU_100, MENU_111, MENU_109,  0x03 , *Fun110}, 
     {{"111 End Time 1      "},/* {"304.回基站时间      "},*/MENU_111, MENU_100, MENU_112, MENU_110,  0x03 , *Fun111}, 
     {{"112 Start Time 2    "},/* {"304.回基站时间      "},*/MENU_112, MENU_100, MENU_113, MENU_111,  0x03 , *Fun112}, 
     {{"113 End Time 2      "},/* {"304.回基站时间      "},*/MENU_113, MENU_100, MENU_114, MENU_112,  0x03 , *Fun113}, 
     {{"114 Start Time 3    "},/* {"304.回基站时间      "},*/MENU_114, MENU_100, MENU_115, MENU_113,  0x03 , *Fun114}, 
     {{"115 End Time 3      "},/* {"304.回基站时间      "},*/MENU_115, MENU_100, MENU_116, MENU_114,  0x03 , *Fun115}, 
     {{"116 Start Time 4    "},/* {"304.回基站时间      "},*/MENU_116, MENU_100, MENU_117, MENU_115,  0x03 , *Fun116}, 
     {{"117 End Time 4      "},/* {"304.回基站时间      "},*/MENU_117, MENU_100, MENU_118, MENU_116,  0x03 , *Fun117}, 
     {{"118 Start Time 5    "},/* {"304.回基站时间      "},*/MENU_118, MENU_100, MENU_119, MENU_117,  0x03 , *Fun118}, 
     {{"119 End Time 5      "},/* {"304.回基站时间      "},*/MENU_119, MENU_100, MENU_120, MENU_118,  0x03 , *Fun119}, 
     {{"120 Start Time 6    "},/* {"304.回基站时间      "},*/MENU_120, MENU_100, MENU_121, MENU_119,  0x03 , *Fun120}, 
     {{"121 End Time 6      "},/* {"304.回基站时间      "},*/MENU_121, MENU_100, MENU_122, MENU_120,  0x03 , *Fun121}, 
     {{"122 Group1 Up       "},/* {"305.组1上呼         "},*/MENU_122, MENU_100, MENU_123, MENU_121,  0x03 , *Fun122}, 
     {{"123 Group1 Down     "},/* {"305.组1下呼         "},*/MENU_123, MENU_100, MENU_124, MENU_122,  0x03 , *Fun123}, 
     {{"124 Group2 Up       "},/* {"305.组2上呼         "},*/MENU_124, MENU_100, MENU_125, MENU_123,  0x03 , *Fun124}, 
     {{"125 Group2 Down     "},/* {"305.组2下呼         "},*/MENU_125, MENU_100, MENU_126, MENU_124,  0x03 , *Fun125}, 
     {{"126 Group3 Up       "},/* {"305.组3上呼         "},*/MENU_126, MENU_100, MENU_127, MENU_125,  0x03 , *Fun126}, 
     {{"127 Group3 Down     "},/* {"305.组3下呼         "},*/MENU_127, MENU_100, MENU_128, MENU_126,  0x03 , *Fun127}, 
     {{"128 Group4 Up       "},/* {"305.组4上呼         "},*/MENU_128, MENU_100, MENU_129, MENU_127,  0x03 , *Fun128}, 
     {{"129 Group4 Down     "},/* {"305.组4下呼         "},*/MENU_129, MENU_100, MENU_130, MENU_128,  0x03 , *Fun129}, 
     {{"130 Group5 Up       "},/* {"305.组5上呼         "},*/MENU_130, MENU_100, MENU_131, MENU_129,  0x03 , *Fun130}, 
     {{"131 Group5 Down     "},/* {"305.组5下呼         "},*/MENU_131, MENU_100, MENU_132, MENU_130,  0x03 , *Fun131}, 
     {{"132 Group6 Up       "},/* {"305.组6上呼         "},*/MENU_132, MENU_100, MENU_133, MENU_131,  0x03 , *Fun132}, 
     {{"133 Group6 Down     "},/* {"305.组6下呼         "},*/MENU_133, MENU_100, MENU_134, MENU_132,  0x03 , *Fun133}, 
     {{"134 Group7 Up       "},/* {"305.组7上呼         "},*/MENU_134, MENU_100, MENU_135, MENU_133,  0x03 , *Fun134}, 
     {{"135 Group7 Down     "},/* {"305.组7下呼         "},*/MENU_135, MENU_100, MENU_136, MENU_134,  0x03 , *Fun135}, 
     {{"136 Group8 Up       "},/* {"305.组8上呼         "},*/MENU_136, MENU_100, MENU_137, MENU_135,  0x03 , *Fun136}, 
     {{"137 Group8 Down     "},/* {"305.组8下呼         "},*/MENU_137, MENU_100, MENU_138, MENU_136,  0x03 , *Fun137}, 
     {{"138 Lift Funtion    "},/* {"305.组8下呼         "},*/MENU_138, MENU_100, MENU_139, MENU_137,  0x03 , *Fun138}, 
     {{"139 Waitfloor 8 lift"},/* {"305.组8下呼         "},*/MENU_139, MENU_100, MENU_140, MENU_138,  0x03 , *Fun139},
     {{"140 Waitfloor 7 lift"},/* {"305.组8下呼         "},*/MENU_140, MENU_100, MENU_141, MENU_139,  0x03 , *Fun140},
     {{"141 Waitfloor 6 lift"},/* {"305.组8下呼         "},*/MENU_141, MENU_100, MENU_142, MENU_140,  0x03 , *Fun141},
     {{"142 Waitfloor 5 lift"},/* {"305.组8下呼         "},*/MENU_142, MENU_100, MENU_143, MENU_141,  0x03 , *Fun142},
     {{"143 Waitfloor 4 lift"},/* {"305.组8下呼         "},*/MENU_143, MENU_100, MENU_144, MENU_142,  0x03 , *Fun143},
     {{"144 Waitfloor 3 lift"},/* {"305.组8下呼         "},*/MENU_144, MENU_100, MENU_145, MENU_143,  0x03 , *Fun144},
     {{"145 Waitfloor 2 lift"},/* {"305.组8下呼         "},*/MENU_145, MENU_100, MENU_146, MENU_144,  0x03 , *Fun145},
     {{"146 Nornal Generator"},/* {"305.组8下呼         "},*/MENU_146, MENU_100, MENU_147, MENU_145,  0x03 , *Fun146},
     {{"147 Fire Generator  "},/* {"305.组8下呼         "},*/MENU_147, MENU_100, MENU_148, MENU_146,  0x03 , *Fun147},
     {{"148 Waitfloor Time  "},/* {"305.组8下呼         "},*/MENU_148, MENU_100, MENU_149, MENU_147,  0x03 , *Fun148},
	   {{"149 System Week     "},/* {"104.系统星期        "},*/MENU_149, MENU_100, MENU_150, MENU_148,  0x03 , *Fun149},		 
	   {{"150 Floor Run Time  "},/* {"104.电梯单层运行时间 "},*/MENU_150, MENU_100, MENU_151, MENU_149,  0x03 , *Fun150},		 
	   {{"151 Run Stop Time   "},/* {"104.电梯停靠时间     "},*/MENU_151, MENU_100, MENU_152, MENU_150,  0x03 , *Fun151},		 
	   {{"152 Run Order Time  "},/* {"104.电梯登记优先时间 "},*/MENU_152, MENU_100, MENU_153, MENU_151,  0x03 , *Fun152},			 

     {{"153 Lift1 Car Order "},/* {"305.组1上呼         "},*/MENU_153, MENU_100, MENU_154, MENU_152,  0x03 , *Fun153},
     {{"154 Lift2 Car Order "},/* {"305.组1上呼         "},*/MENU_154, MENU_100, MENU_155, MENU_153,  0x03 , *Fun154},
     {{"155 Lift3 Car Order "},/* {"305.组1上呼         "},*/MENU_155, MENU_100, MENU_156, MENU_154,  0x03 , *Fun155},
     {{"156 Lift4 Car Order "},/* {"305.组1上呼         "},*/MENU_156, MENU_100, MENU_157, MENU_155,  0x03 , *Fun156},
     {{"157 Lift5 Car Order "},/* {"305.组1上呼         "},*/MENU_157, MENU_100, MENU_158, MENU_156,  0x03 , *Fun157},
     {{"158 Lift6 Car Order "},/* {"305.组1上呼         "},*/MENU_158, MENU_100, MENU_159, MENU_157,  0x03 , *Fun158},
     {{"159 Lift7 Car Order "},/* {"305.组1上呼         "},*/MENU_159, MENU_100, MENU_160, MENU_158,  0x03 , *Fun159},
     {{"160 Lift8 Car Order "},/* {"305.组1上呼         "},*/MENU_160, MENU_100, MENU_161, MENU_159,  0x03 , *Fun160},
		 
     {{"161 Lift1 Up Order  "},/* {"305.组1上呼         "},*/MENU_161, MENU_100, MENU_162, MENU_160,  0x03 , *Fun161},
     {{"162 Lift1 Dn Order  "},/* {"305.组1上呼         "},*/MENU_162, MENU_100, MENU_163, MENU_161,  0x03 , *Fun162},
     {{"163 Lift2 Up Order  "},/* {"305.组1上呼         "},*/MENU_163, MENU_100, MENU_164, MENU_162,  0x03 , *Fun163},
     {{"164 Lift2 Dn Order  "},/* {"305.组1上呼         "},*/MENU_164, MENU_100, MENU_165, MENU_163,  0x03 , *Fun164},
     {{"165 Lift3 Up Order  "},/* {"305.组1上呼         "},*/MENU_165, MENU_100, MENU_166, MENU_164,  0x03 , *Fun165},
     {{"166 Lift3 Dn Order  "},/* {"305.组1上呼         "},*/MENU_166, MENU_100, MENU_167, MENU_165,  0x03 , *Fun166},
     {{"167 Lift4 Up Order  "},/* {"305.组1上呼         "},*/MENU_167, MENU_100, MENU_168, MENU_166,  0x03 , *Fun167},
     {{"168 Lift4 Dn Order  "},/* {"305.组1上呼         "},*/MENU_168, MENU_100, MENU_169, MENU_167,  0x03 , *Fun168},
     {{"169 Lift5 Up Order  "},/* {"305.组1上呼         "},*/MENU_169, MENU_100, MENU_170, MENU_168,  0x03 , *Fun169},
     {{"170 Lift5 Dn Order  "},/* {"305.组1上呼         "},*/MENU_170, MENU_100, MENU_171, MENU_169,  0x03 , *Fun170},
     {{"171 Lift6 Up Order  "},/* {"305.组1上呼         "},*/MENU_171, MENU_100, MENU_172, MENU_170,  0x03 , *Fun171},
     {{"172 Lift6 Dn Order  "},/* {"305.组1上呼         "},*/MENU_172, MENU_100, MENU_173, MENU_171,  0x03 , *Fun172},
     {{"173 Lift7 Up Order  "},/* {"305.组1上呼         "},*/MENU_173, MENU_100, MENU_174, MENU_172,  0x03 , *Fun173},
     {{"174 Lift7 Dn Order  "},/* {"305.组1上呼         "},*/MENU_174, MENU_100, MENU_175, MENU_173,  0x03 , *Fun174},
     {{"175 Lift8 Up Order  "},/* {"305.组1上呼         "},*/MENU_175, MENU_100, MENU_176, MENU_174,  0x03 , *Fun175},
     {{"176 Lift8 Dn Order  "},/* {"305.组1上呼         "},*/MENU_176, MENU_100, MENU_176, MENU_175,  0x03 , *Fun176},
		 
     {{"200 lift 1          "},/* {"200.电梯1           "},*/MENU_201, MENU_001, MENU_300, MENU_100,  0x00 , *Fun200}, 
     {{"201 Address 1       "},/* {"201.地址1           "},*/MENU_201, MENU_200, MENU_202, MENU_201,  0x03 , *Fun201}, 
     {{"202 First Floor 1   "},/* {"202.首层楼层1       "},*/MENU_202, MENU_200, MENU_203, MENU_201,  0x03 , *Fun202}, 
     {{"203 Max Floor 1     "},/* {"203.最高楼层1       "},*/MENU_203, MENU_200, MENU_204, MENU_202,  0x03 , *Fun203}, 
     {{"204 Enable Up 1     "},/* {"204.上呼禁止1       "},*/MENU_204, MENU_200, MENU_205, MENU_203,  0x03 , *Fun204}, 
     {{"205 Enable Down 1   "},/* {"205.下呼禁止1       "},*/MENU_205, MENU_200, MENU_206, MENU_204,  0x03 , *Fun205}, 		
     {{"206 New Flag 1      "},/* {"205.下呼禁止1       "},*/MENU_206, MENU_200, MENU_207, MENU_205,  0x03 , *Fun206}, 
     {{"207 Base Floor 1    "},/* {"205.下呼禁止1       "},*/MENU_207, MENU_200, MENU_208, MENU_206,  0x03 , *Fun207}, 
     {{"208 L1 Up Button Gr1"},/* {"305.组1上呼         "},*/MENU_208, MENU_200, MENU_209, MENU_207,  0x03 , *Fun208}, 
     {{"209 L1 Dn Button Gr1"},/* {"305.组1下呼         "},*/MENU_209, MENU_200, MENU_210, MENU_208,  0x03 , *Fun209}, 
     {{"210 L1 Up Button Gr2"},/* {"305.组2上呼         "},*/MENU_210, MENU_200, MENU_211, MENU_209,  0x03 , *Fun210}, 
     {{"211 L1 Dn Button Gr2"},/* {"305.组2下呼         "},*/MENU_211, MENU_200, MENU_212, MENU_210,  0x03 , *Fun211}, 
     {{"212 L1 Up Button Gr3"},/* {"305.组3上呼         "},*/MENU_212, MENU_200, MENU_213, MENU_211,  0x03 , *Fun212}, 
     {{"213 L1 Dn Button Gr3"},/* {"305.组3下呼         "},*/MENU_213, MENU_200, MENU_214, MENU_212,  0x03 , *Fun213}, 
     {{"214 L1 Up Button Gr4"},/* {"305.组4上呼         "},*/MENU_214, MENU_200, MENU_215, MENU_213,  0x03 , *Fun214}, 
     {{"215 L1 Dn Button Gr4"},/* {"305.组4下呼         "},*/MENU_215, MENU_200, MENU_216, MENU_214,  0x03 , *Fun215}, 
     {{"216 L1 Up Button Gr5"},/* {"305.组5上呼         "},*/MENU_216, MENU_200, MENU_217, MENU_215,  0x03 , *Fun216}, 
     {{"217 L1 Dn Button Gr5"},/* {"305.组5下呼         "},*/MENU_217, MENU_200, MENU_218, MENU_216,  0x03 , *Fun217}, 
     {{"218 L1 Up Button Gr6"},/* {"305.组6上呼         "},*/MENU_218, MENU_200, MENU_219, MENU_217,  0x03 , *Fun218}, 
     {{"219 L1 Dn Button Gr6"},/* {"305.组6下呼         "},*/MENU_219, MENU_200, MENU_220, MENU_218,  0x03 , *Fun219}, 
     {{"220 L1 Up Button Gr7"},/* {"305.组7上呼         "},*/MENU_220, MENU_200, MENU_221, MENU_219,  0x03 , *Fun220}, 
     {{"221 L1 Dn Button Gr7"},/* {"305.组7下呼         "},*/MENU_221, MENU_200, MENU_222, MENU_220,  0x03 , *Fun221}, 
     {{"222 L1 Up Button Gr8"},/* {"305.组8上呼         "},*/MENU_222, MENU_200, MENU_223, MENU_221,  0x03 , *Fun222}, 
     {{"223 L1 Dn Button Gr8"},/* {"305.组8下呼         "},*/MENU_223, MENU_200, MENU_224, MENU_222,  0x03 , *Fun223}, 
     {{"224 L1 Up Group1 En "},/* {"305.组1上呼         "},*/MENU_224, MENU_200, MENU_225, MENU_223,  0x03 , *Fun224}, 
     {{"225 L1 Dn Group1 En "},/* {"305.组1下呼         "},*/MENU_225, MENU_200, MENU_226, MENU_224,  0x03 , *Fun225}, 
     {{"226 L1 Up Group2 En "},/* {"305.组2上呼         "},*/MENU_226, MENU_200, MENU_227, MENU_225,  0x03 , *Fun226}, 
     {{"227 L1 Dn Group2 En "},/* {"305.组2下呼         "},*/MENU_227, MENU_200, MENU_228, MENU_226,  0x03 , *Fun227}, 
     {{"228 L1 Up Group3 En "},/* {"305.组3上呼         "},*/MENU_228, MENU_200, MENU_229, MENU_227,  0x03 , *Fun228}, 
     {{"229 L1 Dn Group3 En "},/* {"305.组3下呼         "},*/MENU_229, MENU_200, MENU_230, MENU_228,  0x03 , *Fun229}, 
     {{"230 L1 Up Group4 En "},/* {"305.组4上呼         "},*/MENU_230, MENU_200, MENU_231, MENU_229,  0x03 , *Fun230}, 
     {{"231 L1 Dn Group4 En "},/* {"305.组4下呼         "},*/MENU_231, MENU_200, MENU_232, MENU_230,  0x03 , *Fun231}, 
     {{"232 L1 Up Group5 En "},/* {"305.组5上呼         "},*/MENU_232, MENU_200, MENU_233, MENU_231,  0x03 , *Fun232}, 
     {{"233 L1 Dn Group5 En "},/* {"305.组5下呼         "},*/MENU_233, MENU_200, MENU_234, MENU_232,  0x03 , *Fun233}, 
     {{"234 L1 Up Group6 En "},/* {"305.组6上呼         "},*/MENU_234, MENU_200, MENU_235, MENU_233,  0x03 , *Fun234}, 
     {{"235 L1 Dn Group6 En "},/* {"305.组6下呼         "},*/MENU_235, MENU_200, MENU_236, MENU_234,  0x03 , *Fun235}, 
     {{"236 L1 Up Group7 En "},/* {"305.组7上呼         "},*/MENU_236, MENU_200, MENU_237, MENU_235,  0x03 , *Fun236}, 
     {{"237 L1 Dn Group7 En "},/* {"305.组7下呼         "},*/MENU_237, MENU_200, MENU_238, MENU_236,  0x03 , *Fun237}, 
     {{"238 L1 Up Group8 En "},/* {"305.组8上呼         "},*/MENU_238, MENU_200, MENU_239, MENU_237,  0x03 , *Fun238}, 
     {{"239 L1 Dn Group8 En "},/* {"305.组8下呼         "},*/MENU_239, MENU_200, MENU_240, MENU_238,  0x03 , *Fun239}, 
     {{"240 Lift1 Time1 Set "},/* {"305.组7上呼         "},*/MENU_240, MENU_200, MENU_241, MENU_239,  0x03 , *Fun240}, 
     {{"241 Lift1 Time2 Set "},/* {"305.组7下呼         "},*/MENU_241, MENU_200, MENU_242, MENU_240,  0x03 , *Fun241}, 
     {{"242 Lift1 Time3 Set "},/* {"305.组8上呼         "},*/MENU_242, MENU_200, MENU_243, MENU_241,  0x03 , *Fun242}, 
     {{"243 Lift1 Time4 Set "},/* {"305.组8下呼         "},*/MENU_243, MENU_200, MENU_244, MENU_242,  0x03 , *Fun243}, 
     {{"244 Lift1 Time5 Set "},/* {"305.组1上呼         "},*/MENU_244, MENU_200, MENU_245, MENU_243,  0x03 , *Fun244}, 
     {{"245 Lift1 Time6 Set "},/* {"305.组1下呼         "},*/MENU_245, MENU_200, MENU_246, MENU_244,  0x03 , *Fun245}, 
     {{"246 Lift1 Time7 Set "},/* {"305.组2上呼         "},*/MENU_246, MENU_200, MENU_247, MENU_245,  0x03 , *Fun246}, 
     {{"247 Lift1 Time8 Set "},/* {"305.组2下呼         "},*/MENU_247, MENU_200, MENU_248, MENU_246,  0x03 , *Fun247}, 
     {{"248 Lift1 Time1 En  "},/* {"305.组3上呼         "},*/MENU_248, MENU_200, MENU_249, MENU_247,  0x03 , *Fun248}, 
     {{"249 Lift1 Time2 En  "},/* {"305.组3下呼         "},*/MENU_249, MENU_200, MENU_250, MENU_248,  0x03 , *Fun249}, 
     {{"250 Lift1 Time3 En  "},/* {"305.组4上呼         "},*/MENU_250, MENU_200, MENU_251, MENU_249,  0x03 , *Fun250}, 
     {{"251 Lift1 Time4 En  "},/* {"305.组4下呼         "},*/MENU_251, MENU_200, MENU_252, MENU_250,  0x03 , *Fun251}, 
     {{"252 Lift1 Time5 En  "},/* {"305.组5上呼         "},*/MENU_252, MENU_200, MENU_253, MENU_251,  0x03 , *Fun252}, 
     {{"253 Lift1 Time6 En  "},/* {"305.组5下呼         "},*/MENU_253, MENU_200, MENU_254, MENU_252,  0x03 , *Fun253}, 
     {{"254 Lift1 Time7 En  "},/* {"305.组6上呼         "},*/MENU_254, MENU_200, MENU_255, MENU_253,  0x03 , *Fun254}, 
     {{"255 Lift1 Time8 En  "},/* {"305.组6下呼         "},*/MENU_255, MENU_200, MENU_255, MENU_254,  0x03 , *Fun255}, 
		 
     {{"300 lift 2          "},/* {"300.电梯2           "},*/MENU_301, MENU_001, MENU_400, MENU_200,  0x00 , *Fun300}, 
     {{"301 Address 2       "},/* {"301.地址2           "},*/MENU_301, MENU_300, MENU_302, MENU_301,  0x03 , *Fun301}, 
     {{"302 First Floor 2   "},/* {"302.首层楼层2       "},*/MENU_302, MENU_300, MENU_303, MENU_301,  0x03 , *Fun302}, 
     {{"303 Max Floor 2     "},/* {"303.最高楼层2       "},*/MENU_303, MENU_300, MENU_304, MENU_302,  0x03 , *Fun303}, 
     {{"304 Enable Up 2     "},/* {"304.上呼禁止2       "},*/MENU_304, MENU_300, MENU_305, MENU_303,  0x03 , *Fun304}, 
     {{"305 Enable Down 2   "},/* {"305.下呼禁止2       "},*/MENU_305, MENU_300, MENU_306, MENU_304,  0x03 , *Fun305}, 
     {{"306 New Flag 2      "},/* {"205.下呼禁止1       "},*/MENU_306, MENU_300, MENU_307, MENU_305,  0x03 , *Fun306}, 
     {{"307 Base Floor 2    "},/* {"205.下呼禁止1       "},*/MENU_307, MENU_300, MENU_308, MENU_306,  0x03 , *Fun307}, 
     {{"308 L2 Up Button Gr1"},/* {"305.组1上呼         "},*/MENU_308, MENU_300, MENU_309, MENU_307,  0x03 , *Fun308}, 
     {{"309 L2 Dn Button Gr1"},/* {"305.组1下呼         "},*/MENU_309, MENU_300, MENU_310, MENU_308,  0x03 , *Fun309}, 
     {{"310 L2 Up Button Gr2"},/* {"305.组2上呼         "},*/MENU_310, MENU_300, MENU_311, MENU_309,  0x03 , *Fun310}, 
     {{"311 L2 Dn Button Gr2"},/* {"305.组2下呼         "},*/MENU_311, MENU_300, MENU_312, MENU_310,  0x03 , *Fun311}, 
     {{"312 L2 Up Button Gr3"},/* {"305.组3上呼         "},*/MENU_312, MENU_300, MENU_313, MENU_311,  0x03 , *Fun312}, 
     {{"313 L2 Dn Button Gr3"},/* {"305.组3下呼         "},*/MENU_313, MENU_300, MENU_314, MENU_312,  0x03 , *Fun313}, 
     {{"314 L2 Up Button Gr4"},/* {"305.组4上呼         "},*/MENU_314, MENU_300, MENU_315, MENU_313,  0x03 , *Fun314}, 
     {{"315 L2 Dn Button Gr4"},/* {"305.组4下呼         "},*/MENU_315, MENU_300, MENU_316, MENU_314,  0x03 , *Fun315}, 
     {{"316 L2 Up Button Gr5"},/* {"305.组5上呼         "},*/MENU_316, MENU_300, MENU_317, MENU_315,  0x03 , *Fun316}, 
     {{"317 L2 Dn Button Gr5"},/* {"305.组5下呼         "},*/MENU_317, MENU_300, MENU_318, MENU_316,  0x03 , *Fun317}, 
     {{"318 L2 Up Button Gr6"},/* {"305.组6上呼         "},*/MENU_318, MENU_300, MENU_319, MENU_317,  0x03 , *Fun318}, 
     {{"319 L2 Dn Button Gr6"},/* {"305.组6下呼         "},*/MENU_319, MENU_300, MENU_320, MENU_318,  0x03 , *Fun319}, 
     {{"320 L2 Up Button Gr7"},/* {"305.组7上呼         "},*/MENU_320, MENU_300, MENU_321, MENU_319,  0x03 , *Fun320}, 
     {{"321 L2 Dn Button Gr7"},/* {"305.组7下呼         "},*/MENU_321, MENU_300, MENU_322, MENU_320,  0x03 , *Fun321}, 
     {{"322 L2 Up Button Gr8"},/* {"305.组8上呼         "},*/MENU_322, MENU_300, MENU_323, MENU_321,  0x03 , *Fun322}, 
     {{"323 L2 Dn Button Gr8"},/* {"305.组8下呼         "},*/MENU_323, MENU_300, MENU_324, MENU_322,  0x03 , *Fun323}, 
     {{"324 L2 Up Group1 En "},/* {"305.组1上呼         "},*/MENU_324, MENU_300, MENU_325, MENU_323,  0x03 , *Fun324}, 
     {{"325 L2 Dn Group1 En "},/* {"305.组1下呼         "},*/MENU_325, MENU_300, MENU_326, MENU_324,  0x03 , *Fun325}, 
     {{"326 L2 Up Group2 En "},/* {"305.组2上呼         "},*/MENU_326, MENU_300, MENU_327, MENU_325,  0x03 , *Fun326}, 
     {{"327 L2 Dn Group2 En "},/* {"305.组2下呼         "},*/MENU_327, MENU_300, MENU_328, MENU_326,  0x03 , *Fun327}, 
     {{"328 L2 Up Group3 En "},/* {"305.组3上呼         "},*/MENU_328, MENU_300, MENU_329, MENU_327,  0x03 , *Fun328}, 
     {{"329 L2 Dn Group3 En "},/* {"305.组3下呼         "},*/MENU_329, MENU_300, MENU_330, MENU_328,  0x03 , *Fun329}, 
     {{"330 L2 Up Group4 En "},/* {"305.组4上呼         "},*/MENU_330, MENU_300, MENU_331, MENU_329,  0x03 , *Fun330}, 
     {{"331 L2 Dn Group4 En "},/* {"305.组4下呼         "},*/MENU_331, MENU_300, MENU_332, MENU_330,  0x03 , *Fun331}, 
     {{"332 L2 Up Group5 En "},/* {"305.组5上呼         "},*/MENU_332, MENU_300, MENU_333, MENU_331,  0x03 , *Fun332}, 
     {{"333 L2 Dn Group5 En "},/* {"305.组5下呼         "},*/MENU_333, MENU_300, MENU_334, MENU_332,  0x03 , *Fun333}, 
     {{"334 L2 Up Group6 En "},/* {"305.组6上呼         "},*/MENU_334, MENU_300, MENU_335, MENU_333,  0x03 , *Fun334}, 
     {{"335 L2 Dn Group6 En "},/* {"305.组6下呼         "},*/MENU_335, MENU_300, MENU_336, MENU_334,  0x03 , *Fun335}, 
     {{"336 L2 Up Group7 En "},/* {"305.组7上呼         "},*/MENU_336, MENU_300, MENU_337, MENU_335,  0x03 , *Fun336}, 
     {{"337 L2 Dn Group7 En "},/* {"305.组7下呼         "},*/MENU_337, MENU_300, MENU_338, MENU_336,  0x03 , *Fun337}, 
     {{"338 L2 Up Group8 En "},/* {"305.组8上呼         "},*/MENU_338, MENU_300, MENU_339, MENU_337,  0x03 , *Fun338}, 
     {{"339 L2 Dn Group8 En "},/* {"305.组8下呼         "},*/MENU_339, MENU_300, MENU_340, MENU_338,  0x03 , *Fun339}, 											 
     {{"340 Lift2 Time1 Set "},/* {"305.组7上呼         "},*/MENU_340, MENU_300, MENU_341, MENU_339,  0x03 , *Fun340}, 
     {{"341 Lift2 Time2 Set "},/* {"305.组7下呼         "},*/MENU_341, MENU_300, MENU_342, MENU_340,  0x03 , *Fun341}, 
     {{"342 Lift2 Time3 Set "},/* {"305.组8上呼         "},*/MENU_342, MENU_300, MENU_343, MENU_341,  0x03 , *Fun342}, 
     {{"343 Lift2 Time4 Set "},/* {"305.组8下呼         "},*/MENU_343, MENU_300, MENU_344, MENU_342,  0x03 , *Fun343}, 
     {{"344 Lift2 Time5 Set "},/* {"305.组1上呼         "},*/MENU_344, MENU_300, MENU_345, MENU_343,  0x03 , *Fun344}, 
     {{"345 Lift2 Time6 Set "},/* {"305.组1下呼         "},*/MENU_345, MENU_300, MENU_346, MENU_344,  0x03 , *Fun345}, 
     {{"346 Lift2 Time7 Set "},/* {"305.组2上呼         "},*/MENU_346, MENU_300, MENU_347, MENU_345,  0x03 , *Fun346}, 
     {{"347 Lift2 Time8 Set "},/* {"305.组2下呼         "},*/MENU_347, MENU_300, MENU_348, MENU_346,  0x03 , *Fun347}, 
     {{"348 Lift2 Time1 En  "},/* {"305.组3上呼         "},*/MENU_348, MENU_300, MENU_349, MENU_347,  0x03 , *Fun348}, 
     {{"349 Lift2 Time2 En  "},/* {"305.组3下呼         "},*/MENU_349, MENU_300, MENU_350, MENU_348,  0x03 , *Fun349}, 
     {{"350 Lift2 Time3 En  "},/* {"305.组4上呼         "},*/MENU_350, MENU_300, MENU_351, MENU_349,  0x03 , *Fun350}, 
     {{"351 Lift2 Time4 En  "},/* {"305.组4下呼         "},*/MENU_351, MENU_300, MENU_352, MENU_350,  0x03 , *Fun351}, 
     {{"352 Lift2 Time5 En  "},/* {"305.组5上呼         "},*/MENU_352, MENU_300, MENU_353, MENU_351,  0x03 , *Fun352}, 
     {{"353 Lift2 Time6 En  "},/* {"305.组5下呼         "},*/MENU_353, MENU_300, MENU_354, MENU_352,  0x03 , *Fun353}, 
     {{"354 Lift2 Time7 En  "},/* {"305.组6上呼         "},*/MENU_354, MENU_300, MENU_355, MENU_353,  0x03 , *Fun354}, 
     {{"355 Lift2 Time8 En  "},/* {"305.组6下呼         "},*/MENU_355, MENU_300, MENU_355, MENU_354,  0x03 , *Fun355}, 

     {{"400 lift 3          "},/* {"400.电梯3           "},*/MENU_401, MENU_001, MENU_500, MENU_300,  0x00 , *Fun400}, 
     {{"401 Address 3       "},/* {"401.地址3           "},*/MENU_401, MENU_400, MENU_402, MENU_401,  0x03 , *Fun401}, 
     {{"402 First Floor 3   "},/* {"402.首层楼层3       "},*/MENU_402, MENU_400, MENU_403, MENU_401,  0x03 , *Fun402}, 
     {{"403 Max Floor 3     "},/* {"403.最高楼层3       "},*/MENU_403, MENU_400, MENU_404, MENU_402,  0x03 , *Fun403}, 
     {{"404 Enable Up 3     "},/* {"404.上呼禁止3       "},*/MENU_404, MENU_400, MENU_405, MENU_403,  0x03 , *Fun404}, 
     {{"405 Enable Down 3   "},/* {"405.下呼禁止3       "},*/MENU_405, MENU_400, MENU_406, MENU_404,  0x03 , *Fun405}, 
     {{"406 New Flag 3      "},/* {"205.下呼禁止1       "},*/MENU_406, MENU_400, MENU_407, MENU_405,  0x03 , *Fun406}, 
     {{"407 Base Floor 3    "},/* {"205.下呼禁止1       "},*/MENU_407, MENU_400, MENU_408, MENU_406,  0x03 , *Fun407}, 
     {{"408 L3 Up Button Gr1"},/* {"305.组1上呼         "},*/MENU_408, MENU_400, MENU_409, MENU_407,  0x03 , *Fun408}, 
     {{"409 L3 Dn Button Gr1"},/* {"305.组1下呼         "},*/MENU_409, MENU_400, MENU_410, MENU_408,  0x03 , *Fun409}, 
     {{"410 L3 Up Button Gr2"},/* {"305.组2上呼         "},*/MENU_410, MENU_400, MENU_411, MENU_409,  0x03 , *Fun410}, 
     {{"411 L3 Dn Button Gr2"},/* {"305.组2下呼         "},*/MENU_411, MENU_400, MENU_412, MENU_410,  0x03 , *Fun411}, 
     {{"412 L3 Up Button Gr3"},/* {"305.组3上呼         "},*/MENU_412, MENU_400, MENU_413, MENU_411,  0x03 , *Fun412}, 
     {{"413 L3 Dn Button Gr3"},/* {"305.组3下呼         "},*/MENU_413, MENU_400, MENU_414, MENU_412,  0x03 , *Fun413}, 
     {{"414 L3 Up Button Gr4"},/* {"305.组4上呼         "},*/MENU_414, MENU_400, MENU_415, MENU_413,  0x03 , *Fun414}, 
     {{"415 L3 Dn Button Gr4"},/* {"305.组4下呼         "},*/MENU_415, MENU_400, MENU_416, MENU_414,  0x03 , *Fun415}, 
     {{"416 L3 Up Button Gr5"},/* {"305.组5上呼         "},*/MENU_416, MENU_400, MENU_417, MENU_415,  0x03 , *Fun416}, 
     {{"417 L3 Dn Button Gr5"},/* {"305.组5下呼         "},*/MENU_417, MENU_400, MENU_418, MENU_416,  0x03 , *Fun417}, 
     {{"418 L3 Up Button Gr6"},/* {"305.组6上呼         "},*/MENU_418, MENU_400, MENU_419, MENU_417,  0x03 , *Fun418}, 
     {{"419 L3 Dn Button Gr6"},/* {"305.组6下呼         "},*/MENU_419, MENU_400, MENU_420, MENU_418,  0x03 , *Fun419}, 
     {{"420 L3 Up Button Gr7"},/* {"305.组7上呼         "},*/MENU_420, MENU_400, MENU_421, MENU_419,  0x03 , *Fun420}, 
     {{"421 L3 Dn Button Gr7"},/* {"305.组7下呼         "},*/MENU_421, MENU_400, MENU_422, MENU_420,  0x03 , *Fun421}, 
     {{"422 L3 Up Button Gr8"},/* {"305.组8上呼         "},*/MENU_422, MENU_400, MENU_423, MENU_421,  0x03 , *Fun422}, 
     {{"423 L3 Dn Button Gr8"},/* {"305.组8下呼         "},*/MENU_423, MENU_400, MENU_424, MENU_422,  0x03 , *Fun423}, 
     {{"424 L3 Up Group1 En "},/* {"305.组1上呼         "},*/MENU_424, MENU_400, MENU_425, MENU_423,  0x03 , *Fun424}, 
     {{"425 L3 Dn Group1 En "},/* {"305.组1下呼         "},*/MENU_425, MENU_400, MENU_426, MENU_424,  0x03 , *Fun425}, 
     {{"426 L3 Up Group2 En "},/* {"305.组2上呼         "},*/MENU_426, MENU_400, MENU_427, MENU_425,  0x03 , *Fun426}, 
     {{"427 L3 Dn Group2 En "},/* {"305.组2下呼         "},*/MENU_427, MENU_400, MENU_428, MENU_426,  0x03 , *Fun427}, 
     {{"428 L3 Up Group3 En "},/* {"305.组3上呼         "},*/MENU_428, MENU_400, MENU_429, MENU_427,  0x03 , *Fun428}, 
     {{"429 L3 Dn Group3 En "},/* {"305.组3下呼         "},*/MENU_429, MENU_400, MENU_430, MENU_428,  0x03 , *Fun429}, 
     {{"430 L3 Up Group4 En "},/* {"305.组4上呼         "},*/MENU_430, MENU_400, MENU_431, MENU_429,  0x03 , *Fun430}, 
     {{"431 L3 Dn Group4 En "},/* {"305.组4下呼         "},*/MENU_431, MENU_400, MENU_432, MENU_430,  0x03 , *Fun431}, 
     {{"432 L3 Up Group5 En "},/* {"305.组5上呼         "},*/MENU_432, MENU_400, MENU_433, MENU_431,  0x03 , *Fun432}, 
     {{"433 L3 Dn Group5 En "},/* {"305.组5下呼         "},*/MENU_433, MENU_400, MENU_434, MENU_432,  0x03 , *Fun433}, 
     {{"434 L3 Up Group6 En "},/* {"305.组6上呼         "},*/MENU_434, MENU_400, MENU_435, MENU_433,  0x03 , *Fun434}, 
     {{"435 L3 Dn Group6 En "},/* {"305.组6下呼         "},*/MENU_435, MENU_400, MENU_436, MENU_434,  0x03 , *Fun435}, 
     {{"436 L3 Up Group7 En "},/* {"305.组7上呼         "},*/MENU_436, MENU_400, MENU_437, MENU_435,  0x03 , *Fun436}, 
     {{"437 L3 Dn Group7 En "},/* {"305.组7下呼         "},*/MENU_437, MENU_400, MENU_438, MENU_436,  0x03 , *Fun437}, 
     {{"438 L3 Up Group8 En "},/* {"305.组8上呼         "},*/MENU_438, MENU_400, MENU_439, MENU_437,  0x03 , *Fun438}, 
     {{"439 L3 Dn Group8 En "},/* {"305.组8下呼         "},*/MENU_439, MENU_400, MENU_440, MENU_438,  0x03 , *Fun439},
     {{"440 Lift3 Time1 Set "},/* {"305.组7上呼         "},*/MENU_440, MENU_400, MENU_441, MENU_439,  0x03 , *Fun440}, 
     {{"441 Lift3 Time2 Set "},/* {"305.组7下呼         "},*/MENU_441, MENU_400, MENU_442, MENU_440,  0x03 , *Fun441}, 
     {{"442 Lift3 Time3 Set "},/* {"305.组8上呼         "},*/MENU_442, MENU_400, MENU_443, MENU_441,  0x03 , *Fun442}, 
     {{"443 Lift3 Time4 Set "},/* {"305.组8下呼         "},*/MENU_443, MENU_400, MENU_444, MENU_442,  0x03 , *Fun443}, 
     {{"444 Lift3 Time5 Set "},/* {"305.组1上呼         "},*/MENU_444, MENU_400, MENU_445, MENU_443,  0x03 , *Fun444}, 
     {{"445 Lift3 Time6 Set "},/* {"305.组1下呼         "},*/MENU_445, MENU_400, MENU_446, MENU_444,  0x03 , *Fun445}, 
     {{"446 Lift3 Time7 Set "},/* {"305.组2上呼         "},*/MENU_446, MENU_400, MENU_447, MENU_445,  0x03 , *Fun446}, 
     {{"447 Lift3 Time8 Set "},/* {"305.组2下呼         "},*/MENU_447, MENU_400, MENU_448, MENU_446,  0x03 , *Fun447}, 
     {{"448 Lift3 Time1 En  "},/* {"305.组3上呼         "},*/MENU_448, MENU_400, MENU_449, MENU_447,  0x03 , *Fun448}, 
     {{"449 Lift3 Time2 En  "},/* {"305.组3下呼         "},*/MENU_449, MENU_400, MENU_450, MENU_448,  0x03 , *Fun449}, 
     {{"450 Lift3 Time3 En  "},/* {"305.组4上呼         "},*/MENU_450, MENU_400, MENU_451, MENU_449,  0x03 , *Fun450}, 
     {{"451 Lift3 Time4 En  "},/* {"305.组4下呼         "},*/MENU_451, MENU_400, MENU_452, MENU_450,  0x03 , *Fun451}, 
     {{"452 Lift3 Time5 En  "},/* {"305.组5上呼         "},*/MENU_452, MENU_400, MENU_453, MENU_451,  0x03 , *Fun452}, 
     {{"453 Lift3 Time6 En  "},/* {"305.组5下呼         "},*/MENU_453, MENU_400, MENU_454, MENU_452,  0x03 , *Fun453}, 
     {{"454 Lift3 Time7 En  "},/* {"305.组6上呼         "},*/MENU_454, MENU_400, MENU_455, MENU_453,  0x03 , *Fun454}, 
     {{"455 Lift3 Time8 En  "},/* {"305.组6下呼         "},*/MENU_455, MENU_400, MENU_455, MENU_454,  0x03 , *Fun455}, 		 

     {{"500 lift 4          "},/* {"500.电梯4           "},*/MENU_501, MENU_001, MENU_600, MENU_400,  0x00 , *Fun500}, 
     {{"501 Address 4       "},/* {"501.地址4           "},*/MENU_501, MENU_500, MENU_502, MENU_501,  0x03 , *Fun501}, 
     {{"502 First Floor 4   "},/* {"502.首层楼层4       "},*/MENU_502, MENU_500, MENU_503, MENU_501,  0x03 , *Fun502}, 
     {{"503 Max Floor 4     "},/* {"503.最高楼层4       "},*/MENU_503, MENU_500, MENU_504, MENU_502,  0x03 , *Fun503}, 
     {{"504 Enable Up 4     "},/* {"504.上呼禁止4       "},*/MENU_504, MENU_500, MENU_505, MENU_503,  0x03 , *Fun504}, 
     {{"505 Enable Down 4   "},/* {"505.下呼禁止4       "},*/MENU_505, MENU_500, MENU_506, MENU_504,  0x03 , *Fun505}, 
     {{"506 New Flag 4      "},/* {"205.下呼禁止1       "},*/MENU_506, MENU_500, MENU_507, MENU_505,  0x03 , *Fun506}, 
     {{"507 Base Floor 4    "},/* {"205.下呼禁止1       "},*/MENU_507, MENU_500, MENU_508, MENU_506,  0x03 , *Fun507}, 
     {{"508 L4 Up Button Gr1"},/* {"305.组1上呼         "},*/MENU_508, MENU_500, MENU_509, MENU_507,  0x03 , *Fun508}, 
     {{"509 L4 Dn Button Gr1"},/* {"305.组1下呼         "},*/MENU_509, MENU_500, MENU_510, MENU_508,  0x03 , *Fun509}, 
     {{"510 L4 Up Button Gr2"},/* {"305.组2上呼         "},*/MENU_510, MENU_500, MENU_511, MENU_509,  0x03 , *Fun510}, 
     {{"511 L4 Dn Button Gr2"},/* {"305.组2下呼         "},*/MENU_511, MENU_500, MENU_512, MENU_510,  0x03 , *Fun511}, 
     {{"512 L4 Up Button Gr3"},/* {"305.组3上呼         "},*/MENU_512, MENU_500, MENU_513, MENU_511,  0x03 , *Fun512}, 
     {{"513 L4 Dn Button Gr3"},/* {"305.组3下呼         "},*/MENU_513, MENU_500, MENU_514, MENU_512,  0x03 , *Fun513}, 
     {{"514 L4 Up Button Gr4"},/* {"305.组4上呼         "},*/MENU_514, MENU_500, MENU_515, MENU_513,  0x03 , *Fun514}, 
     {{"515 L4 Dn Button Gr4"},/* {"305.组4下呼         "},*/MENU_515, MENU_500, MENU_516, MENU_514,  0x03 , *Fun515}, 
     {{"516 L4 Up Button Gr5"},/* {"305.组5上呼         "},*/MENU_516, MENU_500, MENU_517, MENU_515,  0x03 , *Fun516}, 
     {{"517 L4 Dn Button Gr5"},/* {"305.组5下呼         "},*/MENU_517, MENU_500, MENU_518, MENU_516,  0x03 , *Fun517}, 
     {{"518 L4 Up Button Gr6"},/* {"305.组6上呼         "},*/MENU_518, MENU_500, MENU_519, MENU_517,  0x03 , *Fun518}, 
     {{"519 L4 Dn Button Gr6"},/* {"305.组6下呼         "},*/MENU_519, MENU_500, MENU_520, MENU_518,  0x03 , *Fun519}, 
     {{"520 L4 Up Button Gr7"},/* {"305.组7上呼         "},*/MENU_520, MENU_500, MENU_521, MENU_519,  0x03 , *Fun520}, 
     {{"521 L4 Dn Button Gr7"},/* {"305.组7下呼         "},*/MENU_521, MENU_500, MENU_522, MENU_520,  0x03 , *Fun521}, 
     {{"522 L4 Up Button Gr8"},/* {"305.组8上呼         "},*/MENU_522, MENU_500, MENU_523, MENU_521,  0x03 , *Fun522}, 
     {{"523 L4 Dn Button Gr8"},/* {"305.组8下呼         "},*/MENU_523, MENU_500, MENU_524, MENU_522,  0x03 , *Fun523}, 
     {{"524 L4 Up Group1 En "},/* {"305.组1上呼         "},*/MENU_524, MENU_500, MENU_525, MENU_523,  0x03 , *Fun524}, 
     {{"525 L4 Dn Group1 En "},/* {"305.组1下呼         "},*/MENU_525, MENU_500, MENU_526, MENU_524,  0x03 , *Fun525}, 
     {{"526 L4 Up Group2 En "},/* {"305.组2上呼         "},*/MENU_526, MENU_500, MENU_527, MENU_525,  0x03 , *Fun526}, 
     {{"527 L4 Dn Group2 En "},/* {"305.组2下呼         "},*/MENU_527, MENU_500, MENU_528, MENU_526,  0x03 , *Fun527}, 
     {{"528 L4 Up Group3 En "},/* {"305.组3上呼         "},*/MENU_528, MENU_500, MENU_529, MENU_527,  0x03 , *Fun528}, 
     {{"529 L4 Dn Group3 En "},/* {"305.组3下呼         "},*/MENU_529, MENU_500, MENU_530, MENU_528,  0x03 , *Fun529}, 
     {{"530 L4 Up Group4 En "},/* {"305.组4上呼         "},*/MENU_530, MENU_500, MENU_531, MENU_529,  0x03 , *Fun530}, 
     {{"531 L4 Dn Group4 En "},/* {"305.组4下呼         "},*/MENU_531, MENU_500, MENU_532, MENU_530,  0x03 , *Fun531}, 
     {{"532 L4 Up Group5 En "},/* {"305.组5上呼         "},*/MENU_532, MENU_500, MENU_533, MENU_531,  0x03 , *Fun532}, 
     {{"533 L4 Dn Group5 En "},/* {"305.组5下呼         "},*/MENU_533, MENU_500, MENU_534, MENU_532,  0x03 , *Fun533}, 
     {{"534 L4 Up Group6 En "},/* {"305.组6上呼         "},*/MENU_534, MENU_500, MENU_535, MENU_533,  0x03 , *Fun534}, 
     {{"535 L4 Dn Group6 En "},/* {"305.组6下呼         "},*/MENU_535, MENU_500, MENU_536, MENU_534,  0x03 , *Fun535}, 
     {{"536 L4 Up Group7 En "},/* {"305.组7上呼         "},*/MENU_536, MENU_500, MENU_537, MENU_535,  0x03 , *Fun536}, 
     {{"537 L4 Dn Group7 En "},/* {"305.组7下呼         "},*/MENU_537, MENU_500, MENU_538, MENU_536,  0x03 , *Fun537}, 
     {{"538 L4 Up Group8 En "},/* {"305.组8上呼         "},*/MENU_538, MENU_500, MENU_539, MENU_537,  0x03 , *Fun538}, 
     {{"539 L4 Dn Group8 En "},/* {"305.组8下呼         "},*/MENU_539, MENU_500, MENU_540, MENU_538,  0x03 , *Fun539}, 	 
     {{"540 Lift4 Time1 Set "},/* {"305.组7上呼         "},*/MENU_540, MENU_500, MENU_541, MENU_539,  0x03 , *Fun540}, 
     {{"541 Lift4 Time2 Set "},/* {"305.组7下呼         "},*/MENU_541, MENU_500, MENU_542, MENU_540,  0x03 , *Fun541}, 
     {{"542 Lift4 Time3 Set "},/* {"305.组8上呼         "},*/MENU_542, MENU_500, MENU_543, MENU_541,  0x03 , *Fun542}, 
     {{"543 Lift4 Time4 Set "},/* {"305.组8下呼         "},*/MENU_543, MENU_500, MENU_544, MENU_542,  0x03 , *Fun543}, 
     {{"544 Lift4 Time5 Set "},/* {"305.组1上呼         "},*/MENU_544, MENU_500, MENU_545, MENU_543,  0x03 , *Fun544}, 
     {{"545 Lift4 Time6 Set "},/* {"305.组1下呼         "},*/MENU_545, MENU_500, MENU_546, MENU_544,  0x03 , *Fun545}, 
     {{"546 Lift4 Time7 Set "},/* {"305.组2上呼         "},*/MENU_546, MENU_500, MENU_547, MENU_545,  0x03 , *Fun546}, 
     {{"547 Lift4 Time8 Set "},/* {"305.组2下呼         "},*/MENU_547, MENU_500, MENU_548, MENU_546,  0x03 , *Fun547}, 
     {{"548 Lift4 Time1 En  "},/* {"305.组3上呼         "},*/MENU_548, MENU_500, MENU_549, MENU_547,  0x03 , *Fun548}, 
     {{"549 Lift4 Time2 En  "},/* {"305.组3下呼         "},*/MENU_549, MENU_500, MENU_550, MENU_548,  0x03 , *Fun549}, 
     {{"550 Lift4 Time3 En  "},/* {"305.组4上呼         "},*/MENU_550, MENU_500, MENU_551, MENU_549,  0x03 , *Fun550}, 
     {{"551 Lift4 Time4 En  "},/* {"305.组4下呼         "},*/MENU_551, MENU_500, MENU_552, MENU_550,  0x03 , *Fun551}, 
     {{"552 Lift4 Time5 En  "},/* {"305.组5上呼         "},*/MENU_552, MENU_500, MENU_553, MENU_551,  0x03 , *Fun552}, 
     {{"553 Lift4 Time6 En  "},/* {"305.组5下呼         "},*/MENU_553, MENU_500, MENU_554, MENU_552,  0x03 , *Fun553}, 
     {{"554 Lift4 Time7 En  "},/* {"305.组6上呼         "},*/MENU_554, MENU_500, MENU_555, MENU_553,  0x03 , *Fun554}, 
     {{"555 Lift4 Time8 En  "},/* {"305.组6下呼         "},*/MENU_555, MENU_500, MENU_555, MENU_554,  0x03 , *Fun555}, 

     {{"600 lift 5          "},/* {"600.电梯5           "},*/MENU_601, MENU_001, MENU_700, MENU_500,  0x00 , *Fun600}, 
     {{"601 Address 5       "},/* {"601.地址5           "},*/MENU_601, MENU_600, MENU_602, MENU_601,  0x03 , *Fun601}, 
     {{"602 First Floor 5   "},/* {"602.首层楼层5       "},*/MENU_602, MENU_600, MENU_603, MENU_601,  0x03 , *Fun602}, 
     {{"603 Max Floor 5     "},/* {"603.最高楼层5       "},*/MENU_603, MENU_600, MENU_604, MENU_602,  0x03 , *Fun603}, 
     {{"604 Enable Up 5     "},/* {"604.上呼禁止5       "},*/MENU_604, MENU_600, MENU_605, MENU_603,  0x03 , *Fun604}, 
     {{"605 Enable Down 5   "},/* {"605.下呼禁止5       "},*/MENU_605, MENU_600, MENU_606, MENU_604,  0x03 , *Fun605}, 
     {{"606 New Flag 5      "},/* {"205.下呼禁止1       "},*/MENU_606, MENU_600, MENU_607, MENU_605,  0x03 , *Fun606}, 
     {{"607 Base Floor 5    "},/* {"205.下呼禁止1       "},*/MENU_607, MENU_600, MENU_608, MENU_606,  0x03 , *Fun607}, 
     {{"608 L5 Up Button Gr1"},/* {"305.组1上呼         "},*/MENU_608, MENU_600, MENU_609, MENU_607,  0x03 , *Fun608}, 
     {{"609 L5 Dn Button Gr1"},/* {"305.组1下呼         "},*/MENU_609, MENU_600, MENU_610, MENU_608,  0x03 , *Fun609}, 
     {{"610 L5 Up Button Gr2"},/* {"305.组2上呼         "},*/MENU_610, MENU_600, MENU_611, MENU_609,  0x03 , *Fun610}, 
     {{"611 L5 Dn Button Gr2"},/* {"305.组2下呼         "},*/MENU_611, MENU_600, MENU_612, MENU_610,  0x03 , *Fun611}, 
     {{"612 L5 Up Button Gr3"},/* {"305.组3上呼         "},*/MENU_612, MENU_600, MENU_613, MENU_611,  0x03 , *Fun612}, 
     {{"613 L5 Dn Button Gr3"},/* {"305.组3下呼         "},*/MENU_613, MENU_600, MENU_614, MENU_612,  0x03 , *Fun613}, 
     {{"614 L5 Up Button Gr4"},/* {"305.组4上呼         "},*/MENU_614, MENU_600, MENU_615, MENU_613,  0x03 , *Fun614}, 
     {{"615 L5 Dn Button Gr4"},/* {"305.组4下呼         "},*/MENU_615, MENU_600, MENU_616, MENU_614,  0x03 , *Fun615}, 
     {{"616 L5 Up Button Gr5"},/* {"305.组5上呼         "},*/MENU_616, MENU_600, MENU_617, MENU_615,  0x03 , *Fun616}, 
     {{"617 L5 Dn Button Gr5"},/* {"305.组5下呼         "},*/MENU_617, MENU_600, MENU_618, MENU_616,  0x03 , *Fun617}, 
     {{"618 L5 Up Button Gr6"},/* {"305.组6上呼         "},*/MENU_618, MENU_600, MENU_619, MENU_617,  0x03 , *Fun618}, 
     {{"619 L5 Dn Button Gr6"},/* {"305.组6下呼         "},*/MENU_619, MENU_600, MENU_620, MENU_618,  0x03 , *Fun619}, 
     {{"620 L5 Up Button Gr7"},/* {"305.组7上呼         "},*/MENU_620, MENU_600, MENU_621, MENU_619,  0x03 , *Fun620}, 
     {{"621 L5 Dn Button Gr7"},/* {"305.组7下呼         "},*/MENU_621, MENU_600, MENU_622, MENU_620,  0x03 , *Fun621}, 
     {{"622 L5 Up Button Gr8"},/* {"305.组8上呼         "},*/MENU_622, MENU_600, MENU_623, MENU_621,  0x03 , *Fun622}, 
     {{"623 L5 Dn Button Gr8"},/* {"305.组8下呼         "},*/MENU_623, MENU_600, MENU_624, MENU_622,  0x03 , *Fun623}, 
     {{"624 L5 Up Group1 En "},/* {"305.组1上呼         "},*/MENU_624, MENU_600, MENU_625, MENU_623,  0x03 , *Fun624}, 
     {{"625 L5 Dn Group1 En "},/* {"305.组1下呼         "},*/MENU_625, MENU_600, MENU_626, MENU_624,  0x03 , *Fun625}, 
     {{"626 L5 Up Group2 En "},/* {"305.组2上呼         "},*/MENU_626, MENU_600, MENU_627, MENU_625,  0x03 , *Fun626}, 
     {{"627 L5 Dn Group2 En "},/* {"305.组2下呼         "},*/MENU_627, MENU_600, MENU_628, MENU_626,  0x03 , *Fun627}, 
     {{"628 L5 Up Group3 En "},/* {"305.组3上呼         "},*/MENU_628, MENU_600, MENU_629, MENU_627,  0x03 , *Fun628}, 
     {{"629 L5 Dn Group3 En "},/* {"305.组3下呼         "},*/MENU_629, MENU_600, MENU_630, MENU_628,  0x03 , *Fun629}, 
     {{"630 L5 Up Group4 En "},/* {"305.组4上呼         "},*/MENU_630, MENU_600, MENU_631, MENU_629,  0x03 , *Fun630}, 
     {{"631 L5 Dn Group4 En "},/* {"305.组4下呼         "},*/MENU_631, MENU_600, MENU_632, MENU_630,  0x03 , *Fun631}, 
     {{"632 L5 Up Group5 En "},/* {"305.组5上呼         "},*/MENU_632, MENU_600, MENU_633, MENU_631,  0x03 , *Fun632}, 
     {{"633 L5 Dn Group5 En "},/* {"305.组5下呼         "},*/MENU_633, MENU_600, MENU_634, MENU_632,  0x03 , *Fun633}, 
     {{"634 L5 Up Group6 En "},/* {"305.组6上呼         "},*/MENU_634, MENU_600, MENU_635, MENU_633,  0x03 , *Fun634}, 
     {{"635 L5 Dn Group6 En "},/* {"305.组6下呼         "},*/MENU_635, MENU_600, MENU_636, MENU_634,  0x03 , *Fun635}, 
     {{"636 L5 Up Group7 En "},/* {"305.组7上呼         "},*/MENU_636, MENU_600, MENU_637, MENU_635,  0x03 , *Fun636}, 
     {{"637 L5 Dn Group7 En "},/* {"305.组7下呼         "},*/MENU_637, MENU_600, MENU_638, MENU_636,  0x03 , *Fun637}, 
     {{"638 L5 Up Group8 En "},/* {"305.组8上呼         "},*/MENU_638, MENU_600, MENU_639, MENU_637,  0x03 , *Fun638}, 
     {{"639 L5 Dn Group8 En "},/* {"305.组8下呼         "},*/MENU_639, MENU_600, MENU_640, MENU_638,  0x03 , *Fun639},
     {{"640 Lift5 Time1 Set "},/* {"305.组7上呼         "},*/MENU_640, MENU_600, MENU_641, MENU_639,  0x03 , *Fun640}, 
     {{"641 Lift5 Time2 Set "},/* {"305.组7下呼         "},*/MENU_641, MENU_600, MENU_642, MENU_640,  0x03 , *Fun641}, 
     {{"642 Lift5 Time3 Set "},/* {"305.组8上呼         "},*/MENU_642, MENU_600, MENU_643, MENU_641,  0x03 , *Fun642}, 
     {{"643 Lift5 Time4 Set "},/* {"305.组8下呼         "},*/MENU_643, MENU_600, MENU_644, MENU_642,  0x03 , *Fun643}, 
     {{"644 Lift5 Time5 Set "},/* {"305.组1上呼         "},*/MENU_644, MENU_600, MENU_645, MENU_643,  0x03 , *Fun644}, 
     {{"645 Lift5 Time6 Set "},/* {"305.组1下呼         "},*/MENU_645, MENU_600, MENU_646, MENU_644,  0x03 , *Fun645}, 
     {{"646 Lift5 Time7 Set "},/* {"305.组2上呼         "},*/MENU_646, MENU_600, MENU_647, MENU_645,  0x03 , *Fun646}, 
     {{"647 Lift5 Time8 Set "},/* {"305.组2下呼         "},*/MENU_647, MENU_600, MENU_648, MENU_646,  0x03 , *Fun647}, 
     {{"648 Lift5 Time1 En  "},/* {"305.组3上呼         "},*/MENU_648, MENU_600, MENU_649, MENU_647,  0x03 , *Fun648}, 
     {{"649 Lift5 Time2 En  "},/* {"305.组3下呼         "},*/MENU_649, MENU_600, MENU_650, MENU_648,  0x03 , *Fun649}, 
     {{"650 Lift5 Time3 En  "},/* {"305.组4上呼         "},*/MENU_650, MENU_600, MENU_651, MENU_649,  0x03 , *Fun650}, 
     {{"651 Lift5 Time4 En  "},/* {"305.组4下呼         "},*/MENU_651, MENU_600, MENU_652, MENU_650,  0x03 , *Fun651}, 
     {{"652 Lift5 Time5 En  "},/* {"305.组5上呼         "},*/MENU_652, MENU_600, MENU_653, MENU_651,  0x03 , *Fun652}, 
     {{"653 Lift5 Time6 En  "},/* {"305.组5下呼         "},*/MENU_653, MENU_600, MENU_654, MENU_652,  0x03 , *Fun653}, 
     {{"654 Lift5 Time7 En  "},/* {"305.组6上呼         "},*/MENU_654, MENU_600, MENU_655, MENU_653,  0x03 , *Fun654}, 
     {{"655 Lift5 Time8 En  "},/* {"305.组6下呼         "},*/MENU_655, MENU_600, MENU_655, MENU_654,  0x03 , *Fun655}, 
		 
     {{"700 lift 6          "},/* {"700.电梯6           "},*/MENU_701, MENU_001, MENU_800, MENU_600,  0x00 , *Fun700}, 
     {{"701 Address 6       "},/* {"701.地址6           "},*/MENU_701, MENU_700, MENU_702, MENU_701,  0x03 , *Fun701}, 
     {{"702 First Floor 6   "},/* {"702.首层楼层6       "},*/MENU_702, MENU_700, MENU_703, MENU_701,  0x03 , *Fun702}, 
     {{"703 Max Floor 6     "},/* {"703.最高楼层6       "},*/MENU_703, MENU_700, MENU_704, MENU_702,  0x03 , *Fun703}, 
     {{"704 Enable Up 6     "},/* {"704.上呼禁止6       "},*/MENU_704, MENU_700, MENU_705, MENU_703,  0x03 , *Fun704}, 
     {{"705 Enable Down 6   "},/* {"705.下呼禁止6       "},*/MENU_705, MENU_700, MENU_706, MENU_704,  0x03 , *Fun705}, 
     {{"706 New Flag 6      "},/* {"205.下呼禁止1       "},*/MENU_706, MENU_700, MENU_707, MENU_705,  0x03 , *Fun706}, 
     {{"707 Base Floor 6    "},/* {"205.下呼禁止1       "},*/MENU_707, MENU_700, MENU_708, MENU_706,  0x03 , *Fun707}, 
     {{"708 L6 Up Button Gr1"},/* {"305.组1上呼         "},*/MENU_708, MENU_700, MENU_709, MENU_707,  0x03 , *Fun708}, 
     {{"709 L6 Dn Button Gr1"},/* {"305.组1下呼         "},*/MENU_709, MENU_700, MENU_710, MENU_708,  0x03 , *Fun709}, 
     {{"710 L6 Up Button Gr2"},/* {"305.组2上呼         "},*/MENU_710, MENU_700, MENU_711, MENU_709,  0x03 , *Fun710}, 
     {{"711 L6 Dn Button Gr2"},/* {"305.组2下呼         "},*/MENU_711, MENU_700, MENU_712, MENU_710,  0x03 , *Fun711}, 
     {{"712 L6 Up Button Gr3"},/* {"305.组3上呼         "},*/MENU_712, MENU_700, MENU_713, MENU_711,  0x03 , *Fun712}, 
     {{"713 L6 Dn Button Gr3"},/* {"305.组3下呼         "},*/MENU_713, MENU_700, MENU_714, MENU_712,  0x03 , *Fun713}, 
     {{"714 L6 Up Button Gr4"},/* {"305.组4上呼         "},*/MENU_714, MENU_700, MENU_715, MENU_713,  0x03 , *Fun714}, 
     {{"715 L6 Dn Button Gr4"},/* {"305.组4下呼         "},*/MENU_715, MENU_700, MENU_716, MENU_714,  0x03 , *Fun715}, 
     {{"716 L6 Up Button Gr5"},/* {"305.组5上呼         "},*/MENU_716, MENU_700, MENU_717, MENU_715,  0x03 , *Fun716}, 
     {{"717 L6 Dn Button Gr5"},/* {"305.组5下呼         "},*/MENU_717, MENU_700, MENU_718, MENU_716,  0x03 , *Fun717}, 
     {{"718 L6 Up Button Gr6"},/* {"305.组6上呼         "},*/MENU_718, MENU_700, MENU_719, MENU_717,  0x03 , *Fun718}, 
     {{"719 L6 Dn Button Gr6"},/* {"305.组6下呼         "},*/MENU_719, MENU_700, MENU_720, MENU_718,  0x03 , *Fun719}, 
     {{"720 L6 Up Button Gr7"},/* {"305.组7上呼         "},*/MENU_720, MENU_700, MENU_721, MENU_719,  0x03 , *Fun720}, 
     {{"721 L6 Dn Button Gr7"},/* {"305.组7下呼         "},*/MENU_721, MENU_700, MENU_722, MENU_720,  0x03 , *Fun721}, 
     {{"722 L6 Up Button Gr8"},/* {"305.组8上呼         "},*/MENU_722, MENU_700, MENU_723, MENU_721,  0x03 , *Fun722}, 
     {{"723 L6 Dn Button Gr8"},/* {"305.组8下呼         "},*/MENU_723, MENU_700, MENU_724, MENU_722,  0x03 , *Fun723}, 
     {{"724 L6 Up Group1 En "},/* {"305.组1上呼         "},*/MENU_724, MENU_700, MENU_725, MENU_723,  0x03 , *Fun724}, 
     {{"725 L6 Dn Group1 En "},/* {"305.组1下呼         "},*/MENU_725, MENU_700, MENU_726, MENU_724,  0x03 , *Fun725}, 
     {{"726 L6 Up Group2 En "},/* {"305.组2上呼         "},*/MENU_726, MENU_700, MENU_727, MENU_725,  0x03 , *Fun726}, 
     {{"727 L6 Dn Group2 En "},/* {"305.组2下呼         "},*/MENU_727, MENU_700, MENU_728, MENU_726,  0x03 , *Fun727}, 
     {{"728 L6 Up Group3 En "},/* {"305.组3上呼         "},*/MENU_728, MENU_700, MENU_729, MENU_727,  0x03 , *Fun728}, 
     {{"729 L6 Dn Group3 En "},/* {"305.组3下呼         "},*/MENU_729, MENU_700, MENU_730, MENU_728,  0x03 , *Fun729}, 
     {{"730 L6 Up Group4 En "},/* {"305.组4上呼         "},*/MENU_730, MENU_700, MENU_731, MENU_729,  0x03 , *Fun730}, 
     {{"731 L6 Dn Group4 En "},/* {"305.组4下呼         "},*/MENU_731, MENU_700, MENU_732, MENU_730,  0x03 , *Fun731}, 
     {{"732 L6 Up Group5 En "},/* {"305.组5上呼         "},*/MENU_732, MENU_700, MENU_733, MENU_731,  0x03 , *Fun732}, 
     {{"733 L6 Dn Group5 En "},/* {"305.组5下呼         "},*/MENU_733, MENU_700, MENU_734, MENU_732,  0x03 , *Fun733}, 
     {{"734 L6 Up Group6 En "},/* {"305.组6上呼         "},*/MENU_734, MENU_700, MENU_735, MENU_733,  0x03 , *Fun734}, 
     {{"735 L6 Dn Group6 En "},/* {"305.组6下呼         "},*/MENU_735, MENU_700, MENU_736, MENU_734,  0x03 , *Fun735}, 
     {{"736 L6 Up Group7 En "},/* {"305.组7上呼         "},*/MENU_736, MENU_700, MENU_737, MENU_735,  0x03 , *Fun736}, 
     {{"737 L6 Dn Group7 En "},/* {"305.组7下呼         "},*/MENU_737, MENU_700, MENU_738, MENU_736,  0x03 , *Fun737}, 
     {{"738 L6 Up Group8 En "},/* {"305.组8上呼         "},*/MENU_738, MENU_700, MENU_739, MENU_737,  0x03 , *Fun738}, 
     {{"739 L6 Dn Group8 En "},/* {"305.组8下呼         "},*/MENU_739, MENU_700, MENU_740, MENU_738,  0x03 , *Fun739}, 
     {{"740 Lift6 Time1 Set "},/* {"305.组7上呼         "},*/MENU_740, MENU_700, MENU_741, MENU_739,  0x03 , *Fun740}, 
     {{"741 Lift6 Time2 Set "},/* {"305.组7下呼         "},*/MENU_741, MENU_700, MENU_742, MENU_740,  0x03 , *Fun741}, 
     {{"742 Lift6 Time3 Set "},/* {"305.组8上呼         "},*/MENU_742, MENU_700, MENU_743, MENU_741,  0x03 , *Fun742}, 
     {{"743 Lift6 Time4 Set "},/* {"305.组8下呼         "},*/MENU_743, MENU_700, MENU_744, MENU_742,  0x03 , *Fun743}, 
     {{"744 Lift6 Time5 Set "},/* {"305.组1上呼         "},*/MENU_744, MENU_700, MENU_745, MENU_743,  0x03 , *Fun744}, 
     {{"745 Lift6 Time6 Set "},/* {"305.组1下呼         "},*/MENU_745, MENU_700, MENU_746, MENU_744,  0x03 , *Fun745}, 
     {{"746 Lift6 Time7 Set "},/* {"305.组2上呼         "},*/MENU_746, MENU_700, MENU_747, MENU_745,  0x03 , *Fun746}, 
     {{"747 Lift6 Time8 Set "},/* {"305.组2下呼         "},*/MENU_747, MENU_700, MENU_748, MENU_746,  0x03 , *Fun747}, 
     {{"748 Lift6 Time1 En  "},/* {"305.组3上呼         "},*/MENU_748, MENU_700, MENU_749, MENU_747,  0x03 , *Fun748}, 
     {{"749 Lift6 Time2 En  "},/* {"305.组3下呼         "},*/MENU_749, MENU_700, MENU_750, MENU_748,  0x03 , *Fun749}, 
     {{"750 Lift6 Time3 En  "},/* {"305.组4上呼         "},*/MENU_750, MENU_700, MENU_751, MENU_749,  0x03 , *Fun750}, 
     {{"751 Lift6 Time4 En  "},/* {"305.组4下呼         "},*/MENU_751, MENU_700, MENU_752, MENU_750,  0x03 , *Fun751}, 
     {{"752 Lift6 Time5 En  "},/* {"305.组5上呼         "},*/MENU_752, MENU_700, MENU_753, MENU_751,  0x03 , *Fun752}, 
     {{"753 Lift6 Time6 En  "},/* {"305.组5下呼         "},*/MENU_753, MENU_700, MENU_754, MENU_752,  0x03 , *Fun753}, 
     {{"754 Lift6 Time7 En  "},/* {"305.组6上呼         "},*/MENU_754, MENU_700, MENU_755, MENU_753,  0x03 , *Fun754}, 
     {{"755 Lift6 Time8 En  "},/* {"305.组6下呼         "},*/MENU_755, MENU_700, MENU_755, MENU_754,  0x03 , *Fun755}, 

     {{"800 lift 7          "},/* {"800.电梯7           "},*/MENU_801, MENU_001, MENU_900, MENU_700,  0x00 , *Fun800}, 
     {{"801 Address 7       "},/* {"801.地址7           "},*/MENU_801, MENU_800, MENU_802, MENU_801,  0x03 , *Fun801}, 
     {{"802 First Floor 7   "},/* {"802.首层楼层7       "},*/MENU_802, MENU_800, MENU_803, MENU_801,  0x03 , *Fun802}, 
     {{"803 Max Floor 7     "},/* {"803.最高楼层7       "},*/MENU_803, MENU_800, MENU_804, MENU_802,  0x03 , *Fun803}, 
     {{"804 Enable Up 7     "},/* {"804.上呼禁止7       "},*/MENU_804, MENU_800, MENU_805, MENU_803,  0x03 , *Fun804}, 
     {{"805 Enable Down 7   "},/* {"805.下呼禁止7       "},*/MENU_805, MENU_800, MENU_806, MENU_804,  0x03 , *Fun805}, 
     {{"806 New Flag 7      "},/* {"205.下呼禁止1       "},*/MENU_806, MENU_800, MENU_807, MENU_805,  0x03 , *Fun806}, 
     {{"807 Base Floor 7    "},/* {"205.下呼禁止1       "},*/MENU_807, MENU_800, MENU_808, MENU_806,  0x03 , *Fun807}, 
     {{"808 L7 Up Button Gr1"},/* {"305.组1上呼         "},*/MENU_808, MENU_800, MENU_809, MENU_807,  0x03 , *Fun808}, 
     {{"809 L7 Dn Button Gr1"},/* {"305.组1下呼         "},*/MENU_809, MENU_800, MENU_810, MENU_808,  0x03 , *Fun809}, 
     {{"810 L7 Up Button Gr2"},/* {"305.组2上呼         "},*/MENU_810, MENU_800, MENU_811, MENU_809,  0x03 , *Fun810}, 
     {{"811 L7 Dn Button Gr2"},/* {"305.组2下呼         "},*/MENU_811, MENU_800, MENU_812, MENU_810,  0x03 , *Fun811}, 
     {{"812 L7 Up Button Gr3"},/* {"305.组3上呼         "},*/MENU_812, MENU_800, MENU_813, MENU_811,  0x03 , *Fun812}, 
     {{"813 L7 Dn Button Gr3"},/* {"305.组3下呼         "},*/MENU_813, MENU_800, MENU_814, MENU_812,  0x03 , *Fun813}, 
     {{"814 L7 Up Button Gr4"},/* {"305.组4上呼         "},*/MENU_814, MENU_800, MENU_815, MENU_813,  0x03 , *Fun814}, 
     {{"815 L7 Dn Button Gr4"},/* {"305.组4下呼         "},*/MENU_815, MENU_800, MENU_816, MENU_814,  0x03 , *Fun815}, 
     {{"816 L7 Up Button Gr5"},/* {"305.组5上呼         "},*/MENU_816, MENU_800, MENU_817, MENU_815,  0x03 , *Fun816}, 
     {{"817 L7 Dn Button Gr5"},/* {"305.组5下呼         "},*/MENU_817, MENU_800, MENU_818, MENU_816,  0x03 , *Fun817}, 
     {{"818 L7 Up Button Gr6"},/* {"305.组6上呼         "},*/MENU_818, MENU_800, MENU_819, MENU_817,  0x03 , *Fun818}, 
     {{"819 L7 Dn Button Gr6"},/* {"305.组6下呼         "},*/MENU_819, MENU_800, MENU_820, MENU_818,  0x03 , *Fun819}, 
     {{"820 L7 Up Button Gr7"},/* {"305.组7上呼         "},*/MENU_820, MENU_800, MENU_821, MENU_819,  0x03 , *Fun820}, 
     {{"821 L7 Dn Button Gr7"},/* {"305.组7下呼         "},*/MENU_821, MENU_800, MENU_822, MENU_820,  0x03 , *Fun821}, 
     {{"822 L7 Up Button Gr8"},/* {"305.组8上呼         "},*/MENU_822, MENU_800, MENU_823, MENU_821,  0x03 , *Fun822}, 
     {{"823 L7 Dn Button Gr8"},/* {"305.组8下呼         "},*/MENU_823, MENU_800, MENU_824, MENU_822,  0x03 , *Fun823}, 
     {{"824 L7 Up Group1 En "},/* {"305.组1上呼         "},*/MENU_824, MENU_800, MENU_825, MENU_823,  0x03 , *Fun824}, 
     {{"825 L7 Dn Group1 En "},/* {"305.组1下呼         "},*/MENU_825, MENU_800, MENU_826, MENU_824,  0x03 , *Fun825}, 
     {{"826 L7 Up Group2 En "},/* {"305.组2上呼         "},*/MENU_826, MENU_800, MENU_827, MENU_825,  0x03 , *Fun826}, 
     {{"827 L7 Dn Group2 En "},/* {"305.组2下呼         "},*/MENU_827, MENU_800, MENU_828, MENU_826,  0x03 , *Fun827}, 
     {{"828 L7 Up Group3 En "},/* {"305.组3上呼         "},*/MENU_828, MENU_800, MENU_829, MENU_827,  0x03 , *Fun828}, 
     {{"829 L7 Dn Group3 En "},/* {"305.组3下呼         "},*/MENU_829, MENU_800, MENU_830, MENU_828,  0x03 , *Fun829}, 
     {{"830 L7 Up Group4 En "},/* {"305.组4上呼         "},*/MENU_830, MENU_800, MENU_831, MENU_829,  0x03 , *Fun830}, 
     {{"831 L7 Dn Group4 En "},/* {"305.组4下呼         "},*/MENU_831, MENU_800, MENU_832, MENU_830,  0x03 , *Fun831}, 
     {{"832 L7 Up Group5 En "},/* {"305.组5上呼         "},*/MENU_832, MENU_800, MENU_833, MENU_831,  0x03 , *Fun832}, 
     {{"833 L7 Dn Group5 En "},/* {"305.组5下呼         "},*/MENU_833, MENU_800, MENU_834, MENU_832,  0x03 , *Fun833}, 
     {{"834 L7 Up Group6 En "},/* {"305.组6上呼         "},*/MENU_834, MENU_800, MENU_835, MENU_833,  0x03 , *Fun834}, 
     {{"835 L7 Dn Group6 En "},/* {"305.组6下呼         "},*/MENU_835, MENU_800, MENU_836, MENU_834,  0x03 , *Fun835}, 
     {{"836 L7 Up Group7 En "},/* {"305.组7上呼         "},*/MENU_836, MENU_800, MENU_837, MENU_835,  0x03 , *Fun836}, 
     {{"837 L7 Dn Group7 En "},/* {"305.组7下呼         "},*/MENU_837, MENU_800, MENU_838, MENU_836,  0x03 , *Fun837}, 
     {{"838 L7 Up Group8 En "},/* {"305.组8上呼         "},*/MENU_838, MENU_800, MENU_839, MENU_837,  0x03 , *Fun838}, 
     {{"839 L7 Dn Group8 En "},/* {"305.组8下呼         "},*/MENU_839, MENU_800, MENU_839, MENU_838,  0x03 , *Fun839}, 
     {{"840 Lift7 Time1 Set "},/* {"305.组7上呼         "},*/MENU_840, MENU_800, MENU_841, MENU_839,  0x03 , *Fun840}, 
     {{"841 Lift7 Time2 Set "},/* {"305.组7下呼         "},*/MENU_841, MENU_800, MENU_842, MENU_840,  0x03 , *Fun841}, 
     {{"842 Lift7 Time3 Set "},/* {"305.组8上呼         "},*/MENU_842, MENU_800, MENU_843, MENU_841,  0x03 , *Fun842}, 
     {{"843 Lift7 Time4 Set "},/* {"305.组8下呼         "},*/MENU_843, MENU_800, MENU_844, MENU_842,  0x03 , *Fun843}, 
     {{"844 Lift7 Time5 Set "},/* {"305.组1上呼         "},*/MENU_844, MENU_800, MENU_845, MENU_843,  0x03 , *Fun844}, 
     {{"845 Lift7 Time6 Set "},/* {"305.组1下呼         "},*/MENU_845, MENU_800, MENU_846, MENU_844,  0x03 , *Fun845}, 
     {{"846 Lift7 Time7 Set "},/* {"305.组2上呼         "},*/MENU_846, MENU_800, MENU_847, MENU_845,  0x03 , *Fun846}, 
     {{"847 Lift7 Time8 Set "},/* {"305.组2下呼         "},*/MENU_847, MENU_800, MENU_848, MENU_846,  0x03 , *Fun847}, 
     {{"848 Lift7 Time1 En  "},/* {"305.组3上呼         "},*/MENU_848, MENU_800, MENU_849, MENU_847,  0x03 , *Fun848}, 
     {{"849 Lift7 Time2 En  "},/* {"305.组3下呼         "},*/MENU_849, MENU_800, MENU_850, MENU_848,  0x03 , *Fun849}, 
     {{"850 Lift7 Time3 En  "},/* {"305.组4上呼         "},*/MENU_850, MENU_800, MENU_851, MENU_849,  0x03 , *Fun850}, 
     {{"851 Lift7 Time4 En  "},/* {"305.组4下呼         "},*/MENU_851, MENU_800, MENU_852, MENU_850,  0x03 , *Fun851}, 
     {{"852 Lift7 Time5 En  "},/* {"305.组5上呼         "},*/MENU_852, MENU_800, MENU_853, MENU_851,  0x03 , *Fun852}, 
     {{"853 Lift7 Time6 En  "},/* {"305.组5下呼         "},*/MENU_853, MENU_800, MENU_854, MENU_852,  0x03 , *Fun853}, 
     {{"854 Lift7 Time7 En  "},/* {"305.组6上呼         "},*/MENU_854, MENU_800, MENU_855, MENU_853,  0x03 , *Fun854}, 
     {{"855 Lift7 Time8 En  "},/* {"305.组6下呼         "},*/MENU_855, MENU_800, MENU_855, MENU_854,  0x03 , *Fun855}, 

     {{"900 lift 8          "},/* {"900.电梯8           "},*/MENU_901, MENU_001, MENU_900, MENU_800,  0x00 , *Fun900},
	   {{"901 Address 8       "},/* {"901.地址8           "},*/MENU_901, MENU_900, MENU_902, MENU_901,  0x03 , *Fun901}, 
     {{"902 First Floor 8   "},/* {"902.首层楼层8       "},*/MENU_902, MENU_900, MENU_903, MENU_901,  0x03 , *Fun902}, 
     {{"903 Max Floor 8     "},/* {"903.最高楼层8       "},*/MENU_903, MENU_900, MENU_904, MENU_902,  0x03 , *Fun903}, 
     {{"904 Enable Up 8     "},/* {"904.上呼禁止8       "},*/MENU_904, MENU_900, MENU_905, MENU_903,  0x03 , *Fun904}, 
     {{"905 Enable Down 8   "},/* {"905.下呼禁止8       "},*/MENU_905, MENU_900, MENU_906, MENU_904,  0x03 , *Fun905}, 
     {{"906 New Flag 8      "},/* {"205.下呼禁止1       "},*/MENU_906, MENU_900, MENU_907, MENU_905,  0x03 , *Fun906}, 
     {{"907 Base Floor 8    "},/* {"205.下呼禁止1       "},*/MENU_907, MENU_900, MENU_908, MENU_906,  0x03 , *Fun907}, 
     {{"908 L8 Up Button Gr1"},/* {"305.组1上呼         "},*/MENU_908, MENU_900, MENU_909, MENU_907,  0x03 , *Fun908}, 
     {{"909 L8 Dn Button Gr1"},/* {"305.组1下呼         "},*/MENU_909, MENU_900, MENU_910, MENU_908,  0x03 , *Fun909}, 
     {{"910 L8 Up Button Gr2"},/* {"305.组2上呼         "},*/MENU_910, MENU_900, MENU_911, MENU_909,  0x03 , *Fun910}, 
     {{"911 L8 Dn Button Gr2"},/* {"305.组2下呼         "},*/MENU_911, MENU_900, MENU_912, MENU_910,  0x03 , *Fun911}, 
     {{"912 L8 Up Button Gr3"},/* {"305.组3上呼         "},*/MENU_912, MENU_900, MENU_913, MENU_911,  0x03 , *Fun912}, 
     {{"913 L8 Dn Button Gr3"},/* {"305.组3下呼         "},*/MENU_913, MENU_900, MENU_914, MENU_912,  0x03 , *Fun913}, 
     {{"914 L8 Up Button Gr4"},/* {"305.组4上呼         "},*/MENU_914, MENU_900, MENU_915, MENU_913,  0x03 , *Fun914}, 
     {{"915 L8 Dn Button Gr4"},/* {"305.组4下呼         "},*/MENU_915, MENU_900, MENU_916, MENU_914,  0x03 , *Fun915}, 
     {{"916 L8 Up Button Gr5"},/* {"305.组5上呼         "},*/MENU_916, MENU_900, MENU_917, MENU_915,  0x03 , *Fun916}, 
     {{"917 L8 Dn Button Gr5"},/* {"305.组5下呼         "},*/MENU_917, MENU_900, MENU_918, MENU_916,  0x03 , *Fun917}, 
     {{"918 L8 Up Button Gr6"},/* {"305.组6上呼         "},*/MENU_918, MENU_900, MENU_919, MENU_917,  0x03 , *Fun918}, 
     {{"919 L8 Dn Button Gr6"},/* {"305.组6下呼         "},*/MENU_919, MENU_900, MENU_920, MENU_918,  0x03 , *Fun919}, 
     {{"920 L8 Up Button Gr7"},/* {"305.组7上呼         "},*/MENU_920, MENU_900, MENU_921, MENU_919,  0x03 , *Fun920}, 
     {{"921 L8 Dn Button Gr7"},/* {"305.组7下呼         "},*/MENU_921, MENU_900, MENU_922, MENU_920,  0x03 , *Fun921}, 
     {{"922 L8 Up Button Gr8"},/* {"305.组8上呼         "},*/MENU_922, MENU_900, MENU_923, MENU_921,  0x03 , *Fun922}, 
     {{"923 L8 Dn Button Gr8"},/* {"305.组8下呼         "},*/MENU_923, MENU_900, MENU_924, MENU_922,  0x03 , *Fun923}, 
     {{"924 L8 Up Group1 En "},/* {"305.组1上呼         "},*/MENU_924, MENU_900, MENU_925, MENU_923,  0x03 , *Fun924}, 
     {{"925 L8 Dn Group1 En "},/* {"305.组1下呼         "},*/MENU_925, MENU_900, MENU_926, MENU_924,  0x03 , *Fun925}, 
     {{"926 L8 Up Group2 En "},/* {"305.组2上呼         "},*/MENU_926, MENU_900, MENU_927, MENU_925,  0x03 , *Fun926}, 
     {{"927 L8 Dn Group2 En "},/* {"305.组2下呼         "},*/MENU_927, MENU_900, MENU_928, MENU_926,  0x03 , *Fun927}, 
     {{"928 L8 Up Group3 En "},/* {"305.组3上呼         "},*/MENU_928, MENU_900, MENU_929, MENU_927,  0x03 , *Fun928}, 
     {{"929 L8 Dn Group3 En "},/* {"305.组3下呼         "},*/MENU_929, MENU_900, MENU_930, MENU_928,  0x03 , *Fun929}, 
     {{"930 L8 Up Group4 En "},/* {"305.组4上呼         "},*/MENU_930, MENU_900, MENU_931, MENU_929,  0x03 , *Fun930}, 
     {{"931 L8 Dn Group4 En "},/* {"305.组4下呼         "},*/MENU_931, MENU_900, MENU_932, MENU_930,  0x03 , *Fun931}, 
     {{"932 L8 Up Group5 En "},/* {"305.组5上呼         "},*/MENU_932, MENU_900, MENU_933, MENU_931,  0x03 , *Fun932}, 
     {{"933 L8 Dn Group5 En "},/* {"305.组5下呼         "},*/MENU_933, MENU_900, MENU_934, MENU_932,  0x03 , *Fun933}, 
     {{"934 L8 Up Group6 En "},/* {"305.组6上呼         "},*/MENU_934, MENU_900, MENU_935, MENU_933,  0x03 , *Fun934}, 
     {{"935 L8 Dn Group6 En "},/* {"305.组6下呼         "},*/MENU_935, MENU_900, MENU_936, MENU_934,  0x03 , *Fun935}, 
     {{"936 L8 Up Group7 En "},/* {"305.组7上呼         "},*/MENU_936, MENU_900, MENU_937, MENU_935,  0x03 , *Fun936}, 
     {{"937 L8 Dn Group7 En "},/* {"305.组7下呼         "},*/MENU_937, MENU_900, MENU_938, MENU_936,  0x03 , *Fun937}, 
     {{"938 L8 Up Group8 En "},/* {"305.组8上呼         "},*/MENU_938, MENU_900, MENU_939, MENU_937,  0x03 , *Fun938}, 
     {{"939 L8 Dn Group8 En "},/* {"305.组8下呼         "},*/MENU_939, MENU_900, MENU_939, MENU_938,  0x03 , *Fun939}, 
     {{"940 Lift8 Time1 Set "},/* {"305.组7上呼         "},*/MENU_940, MENU_900, MENU_941, MENU_939,  0x03 , *Fun940}, 
     {{"941 Lift8 Time2 Set "},/* {"305.组7下呼         "},*/MENU_941, MENU_900, MENU_942, MENU_940,  0x03 , *Fun941}, 
     {{"942 Lift8 Time3 Set "},/* {"305.组8上呼         "},*/MENU_942, MENU_900, MENU_943, MENU_941,  0x03 , *Fun942}, 
     {{"943 Lift8 Time4 Set "},/* {"305.组8下呼         "},*/MENU_943, MENU_900, MENU_944, MENU_942,  0x03 , *Fun943}, 
     {{"944 Lift8 Time5 Set "},/* {"305.组1上呼         "},*/MENU_944, MENU_900, MENU_945, MENU_943,  0x03 , *Fun944}, 
     {{"945 Lift8 Time6 Set "},/* {"305.组1下呼         "},*/MENU_945, MENU_900, MENU_946, MENU_944,  0x03 , *Fun945}, 
     {{"946 Lift8 Time7 Set "},/* {"305.组2上呼         "},*/MENU_946, MENU_900, MENU_947, MENU_945,  0x03 , *Fun946}, 
     {{"947 Lift8 Time8 Set "},/* {"305.组2下呼         "},*/MENU_947, MENU_900, MENU_948, MENU_946,  0x03 , *Fun947}, 
     {{"948 Lift8 Time1 En  "},/* {"305.组3上呼         "},*/MENU_948, MENU_900, MENU_949, MENU_947,  0x03 , *Fun948}, 
     {{"949 Lift8 Time2 En  "},/* {"305.组3下呼         "},*/MENU_949, MENU_900, MENU_950, MENU_948,  0x03 , *Fun949}, 
     {{"950 Lift8 Time3 En  "},/* {"305.组4上呼         "},*/MENU_950, MENU_900, MENU_951, MENU_949,  0x03 , *Fun950}, 
     {{"951 Lift8 Time4 En  "},/* {"305.组4下呼         "},*/MENU_951, MENU_900, MENU_952, MENU_950,  0x03 , *Fun951}, 
     {{"952 Lift8 Time5 En  "},/* {"305.组5上呼         "},*/MENU_952, MENU_900, MENU_953, MENU_951,  0x03 , *Fun952}, 
     {{"953 Lift8 Time6 En  "},/* {"305.组5下呼         "},*/MENU_953, MENU_900, MENU_954, MENU_952,  0x03 , *Fun953}, 
     {{"954 Lift8 Time7 En  "},/* {"305.组6上呼         "},*/MENU_954, MENU_900, MENU_955, MENU_953,  0x03 , *Fun954}, 
     {{"955 Lift8 Time8 En  "},/* {"305.组6下呼         "},*/MENU_955, MENU_900, MENU_955, MENU_954,  0x03 , *Fun955}, 
};
/*--------------------------------------------------------------------------------------------------*/
void copy(DWORD *P1, const BYTE *P2 , DWORD N)
{
   DWORD i;
   for(i=0; i<N; i++) {
      (*P1) = (*P2);
	     P1++;
	     P2++;
   }
}

void Clr(DWORD *P1, DWORD N)
{
   DWORD i;
   for(i=0; i<N; i++) {
      (*P1) = ' ';
	  P1++;
   }
}
/*--------------------------------------------------------------------------------------------------*/
void Menu_Ini( void )
{
   LCD_Init();
   CurMenuIndex = MENU_000;
   OldMenuIndex = MENU_001;
   KeyFuncPtr = KeyTab[CurMenuIndex].CurrentOperate;
   NowKeyValue = 0;  //当前按钮值
   OldKeyValue = 0;  //之前按钮值
   Modify = 0;       //参数修改
   ModifyID = 0;     //参数数据位
   PasswordOK[0] = 0;//密码有效标志
   PasswordOK[1] = 0;//密码有效标志
   DispOn500ms = 0;  //闪嗦时间
   PasswordTime = 0; //密码有效时间
   BackLightTime = 0;//背光亮时间 

   NowKeyTimer = 0;  //当前按钮值
   UpKeyTime = 0;    //
   DnKeyTime = 0;    //
   KeyTime = 0;      //背光亮时间
}
/*--------------------------------------------------------------------------------------------------*/
//菜单主程序(1ms)
DWORD fastMenu = 0;
void MenuMain( void )
{
   DWORD i;
   i = KEY_NULL;
   if(ReadKeyExit() == 0)  { i |= KEY_ESC; }
   if(ReadKeyUp() == 0)    { i |= KEY_UP; }
   if(ReadKeyDn() == 0)    { i |= KEY_DOWN; }
   if(ReadKeyEnter() == 0) { i |= KEY_OK; }

   if(i==NowKeyValue)     { NowKeyTimer = 0; }
   else if(NowKeyTimer<5) { NowKeyTimer++; }
   else { 
      NowKeyTimer = 0;
	    NowKeyValue = i; 
   }
/*---------------------------------------------*/
   if (CurMenuIndex ==MENU_000) {
			 if(NowKeyValue == KEY_UP) {//切换显示
					SwitchDisp = 1;
			 }
			 if((NowKeyValue == KEY_DOWN)||(NowKeyValue == KEY_ESC)) {
					SwitchDisp = 0;
			 }
			 if(NowKeyValue == (KEY_UP|KEY_ESC)) {
					fastMenu = MENU_122;
			 }
			 if(NowKeyValue == (KEY_DOWN|KEY_ESC)) {
					fastMenu = MENU_153;
			 }
			 if((NowKeyValue == KEY_NULL)&&(fastMenu>0)) {
			    CurMenuIndex = fastMenu;
				  KeyFuncPtr = KeyTab[CurMenuIndex].CurrentOperate;
				  fastMenu = 0;
			 }
	 }
/*---------------------------------------------*/		  
   if(NowKeyValue == KEY_UP) {
      if(UpKeyTime<0xffffffff) { UpKeyTime++; }
   }
   else { UpKeyTime = 0; }
   if(NowKeyValue == KEY_DOWN) {
      if(DnKeyTime<0xffffffff) { DnKeyTime++; }
   }
   else { DnKeyTime = 0; }

   if((UpKeyTime>3000)||(DnKeyTime>3000)) { i = 3; }
   else if((UpKeyTime>2000)||(DnKeyTime>2000)) { i = 2; }
   else if((UpKeyTime>1000)||(DnKeyTime>1000)) { i = 1; }
   else { i = 0; }
	 
   if((Modify > 0)&&(i>0)) {
      if( ((KeyTime>200)&&(i==1)) ||
				  ((KeyTime>100)&&(i==2)) ||
				  ((KeyTime>50)&&(i==3)) ) { 
	       KeyTime = 0;
		     OldKeyValue = KEY_NULL;
	    }
	    else { KeyTime++; }
   }

   if((++Menu100msDelay)>100) {
      Menu100msDelay = 0;
		  DispOn500ms += 2;
		  if ((DispOn500ms > 10)||(NowKeyValue != KEY_NULL)) { DispOn500ms = 0; }//1S
      
      if (PasswordTime < 600*15) { PasswordTime++; } //15min
      else { 
	       PasswordOK[0] = 0; 
	       PasswordOK[1] = 0; 
	    }

      if(BackLightTime < 600*30) {//30min 
         BackLightTime++;    
	       ClrLcdLed(); 
//	     SetLcdLed();
      }    
      else { 
	       SetLcdLed();
//	     ClrLcdLed(); 
      }
   }
   else if (NowKeyValue == OldKeyValue) { return; }

   if(NowKeyValue != OldKeyValue) {
      PasswordTime = 0;   //延长密码时间
      BackLightTime = 0;  //延长背光时间
   }

//   #ifdef _debug 
//   PasswordOK[0] = 1;
//   PasswordOK[1] = 1;
//   #endif

   //菜单选择
   if((Modify == 0)&&(NowKeyValue != OldKeyValue)) { 
      OldKeyValue = NowKeyValue;
	  if(NowKeyValue == KEY_NULL) { 
	     return; 
	  }
	    LCD_Init();
      switch(NowKeyValue) {
	     case   KEY_OK: if((((KeyTab[CurMenuIndex].ModifyEn)&0x0f) == 1)||
						   ((((KeyTab[CurMenuIndex].ModifyEn)&0x0f) == 3)&(PasswordOK[0] == 1))||
						   ((((KeyTab[CurMenuIndex].ModifyEn)&0x0f) > 1)&(PasswordOK[1] == 1))
						   ){
		                //if(KeyTab[CurMenuIndex].ModifyEn == 1) {
                           Modify = 1;    //参数修改标志
                           break;
                        } 
                        while(1) {//有第二密码显示
		                   CurMenuIndex = KeyTab[CurMenuIndex].OkMenuID;
						   if(((KeyTab[CurMenuIndex].ModifyEn)&0xf0) == 0) { break; }
		                } 
                        break;
	     case   KEY_UP: while(1) {//有第二密码显示
		                   CurMenuIndex = KeyTab[CurMenuIndex].UpMenuID;
						   if(((KeyTab[CurMenuIndex].ModifyEn)&0xf0) == 0) { break; }
						}
		                break;
         case KEY_DOWN: while(1) {//有第二密码显示
		                   CurMenuIndex = KeyTab[CurMenuIndex].DownMenuID;
						   if(((KeyTab[CurMenuIndex].ModifyEn)&0xf0) == 0) { break; }
						}
		 			    break;
	     case  KEY_ESC: while(1) {//有第二密码显示
		                   CurMenuIndex = KeyTab[CurMenuIndex].EscMenuID;
						   if(((KeyTab[CurMenuIndex].ModifyEn)&0xf0) == 0) { break; }
						}
					    break;
         default:	    break;
      }
      KeyFuncPtr = KeyTab[CurMenuIndex].CurrentOperate;
   }
   if(OldMenuIndex != CurMenuIndex) {
      OldMenuIndex = CurMenuIndex;
	  ModifyID = 0;  //修改参数的第几位
	  ModifyData = 0; 
	  copy(DispRam, (KeyTab[CurMenuIndex].ETab), 20); //复制界面
	  Clr(&DispRam[20], 60);
   }
   if(Modified==1) { //已经修改参数标志
      SaveStaticPara();
      Modified = 0;
   }

   (*KeyFuncPtr)(); //显示或修改数据
   LCD_Disp();
}
/*--------------------------------------------------------------------------------------------------*/
const BYTE UPDN_TAB[4]= {
     ' ',
     1,
     2,
     ' ',
};

const BYTE DOOR_TAB[8]= {
     ' ',
     'C',
     'c',
     'o',
     'O',
     ' ',
};
void DispState(void)
{
  DispRam[20] = '1';
  DispRam[21] = ':';							 
  if((LiftState[0]&0xc0)==0x80)	{ DispRam[22] = 0; }
  else if((LiftState[0]&0xc0)==0x40) { DispRam[22] = 1; }
  else { DispRam[22] = ' '; }
  DispRam[23] = (LiftABSFloor[0]/10)+'0';
  DispRam[24] = (LiftABSFloor[0]%10)+'0';							 

  DispRam[26] = '2';
  DispRam[27] = ':';
  if((LiftState[1]&0xc0)==0x80)	{ DispRam[28] = 0; }
  else if((LiftState[1]&0xc0)==0x40) { DispRam[28] = 1; }
  else { DispRam[28] = ' '; }
  DispRam[29] = (LiftABSFloor[1]/10)+'0';
  DispRam[30] = (LiftABSFloor[1]%10)+'0';							 

  DispRam[32] = '3';
  DispRam[33] = ':';
  if((LiftState[2]&0xc0)==0x80)	{ DispRam[34] = 0; }
  else if((LiftState[2]&0xc0)==0x40) { DispRam[34] = 1; }
  else { DispRam[34] = ' '; }
  DispRam[35] = (LiftABSFloor[2]/10)+'0';
  DispRam[36] = (LiftABSFloor[2]%10)+'0';							 

  DispRam[40] = '4';
  DispRam[41] = ':';
  if((LiftState[3]&0xc0)==0x80)	{ DispRam[42] = 0; }
  else if((LiftState[3]&0xc0)==0x40) { DispRam[42] = 1; }
  else { DispRam[42] = ' '; }
  DispRam[43] = (LiftABSFloor[3]/10)+'0';
  DispRam[44] = (LiftABSFloor[3]%10)+'0';							 

  DispRam[46] = '5';
  DispRam[47] = ':';
  if((LiftState[4]&0xc0)==0x80)	{ DispRam[48] = 0; }
  else if((LiftState[4]&0xc0)==0x40) { DispRam[48] = 1; }
  else { DispRam[48] = ' '; }
  DispRam[49] = (LiftABSFloor[4]/10)+'0';
  DispRam[50] = (LiftABSFloor[4]%10)+'0';							 

  DispRam[52] = '6';
  DispRam[53] = ':';
  if((LiftState[5]&0xc0)==0x80)	{ DispRam[54] = 0; }
  else if((LiftState[5]&0xc0)==0x40) { DispRam[54] = 1; }
  else { DispRam[54] = ' '; }
  DispRam[55] = (LiftABSFloor[5]/10)+'0';
  DispRam[56] = (LiftABSFloor[5]%10)+'0';							 

  DispRam[60] = '7';
  DispRam[61] = ':';
  if((LiftState[6]&0xc0)==0x80)	{ DispRam[62] = 0; }
  else if((LiftState[6]&0xc0)==0x40) { DispRam[62] = 1; }
  else { DispRam[62] = ' '; }			 
  DispRam[63] = (LiftABSFloor[6]/10)+'0';
  DispRam[64] = (LiftABSFloor[6]%10)+'0';							 

  DispRam[66] = '8';
  DispRam[67] = ':';							 
  if((LiftState[7]&0xc0)==0x80)	{ DispRam[68] = 0; }
  else if((LiftState[7]&0xc0)==0x40) { DispRam[68] = 1; }
  else { DispRam[68] = ' '; }			 
  DispRam[69] = (LiftABSFloor[7]/10)+'0';
  DispRam[70] = (LiftABSFloor[7]%10)+'0';							 
}

/*
void DispState2(void)
{
  DWORD i;
  if((LiftState[0]&0xc0)==0x80)	{ DispRam[0] = 0; }
  else if((LiftState[0]&0xc0)==0x40) { DispRam[0] = 1; }
  else { DispRam[0] = ' '; }
  DispRam[2] = 'R';
  DispRam[3] = ':';
  DispRam[4] = (LiftRunFloor[0]/10)+'0';
  DispRam[5] = (LiftRunFloor[0]%10)+'0';							 
  DispRam[7] = 'D';
  DispRam[8] = ':';
  DispRam[9] = (LiftEnDecFloor[0]/10)+'0';
  DispRam[10] = (LiftEnDecFloor[0]%10)+'0';							 
  DispRam[12] = 'N';
  DispRam[13] = ':';
  DispRam[14] = (LiftNowFloor[0]/10)+'0';
  DispRam[15] = (LiftNowFloor[0]%10)+'0';							 

  DispRam[17] = (TL1/100)+'0';  i = TL1%100; 
  DispRam[18] = (i/10)+'0';							 
  DispRam[19] = (i%10)+'0';							 

  if((LiftState[1]&0xc0)==0x80)	{ DispRam[20] = 0; }
  else if((LiftState[1]&0xc0)==0x40) { DispRam[20] = 1; }
  else { DispRam[20] = ' '; }
  DispRam[20+2] = 'R';
  DispRam[20+3] = ':';
  DispRam[20+4] = (LiftRunFloor[1]/10)+'0';
  DispRam[20+5] = (LiftRunFloor[1]%10)+'0';							 
  DispRam[20+7] = 'D';
  DispRam[20+8] = ':';
  DispRam[20+9] = (LiftEnDecFloor[1]/10)+'0';
  DispRam[20+10] = (LiftEnDecFloor[1]%10)+'0';							 
  DispRam[20+12] = 'N';
  DispRam[20+13] = ':';
  DispRam[20+14] = (LiftNowFloor[1]/10)+'0';
  DispRam[20+15] = (LiftNowFloor[1]%10)+'0';							 

  DispRam[20+17] = (TL2/100)+'0';  i = TL2%100; 
  DispRam[20+18] = (i/10)+'0';							 
  DispRam[20+19] = (i%10)+'0';							 

  if((LiftState[2]&0xc0)==0x80)	{ DispRam[40] = 0; }
  else if((LiftState[2]&0xc0)==0x40) { DispRam[40] = 1; }
  else { DispRam[40] = ' '; }
  DispRam[40+2] = 'R';
  DispRam[40+3] = ':';
  DispRam[40+4] = (LiftRunFloor[2]/10)+'0';
  DispRam[40+5] = (LiftRunFloor[2]%10)+'0';							 
  DispRam[40+7] = 'D';
  DispRam[40+8] = ':';
  DispRam[40+9] = (LiftEnDecFloor[2]/10)+'0';
  DispRam[40+10] = (LiftEnDecFloor[2]%10)+'0';							 
  DispRam[40+12] = 'N';
  DispRam[40+13] = ':';
  DispRam[40+14] = (LiftNowFloor[2]/10)+'0';
  DispRam[40+15] = (LiftNowFloor[2]%10)+'0';

  DispRam[40+17] = (TL3/100)+'0';  i = TL3%100; 
  DispRam[40+18] = (i/10)+'0';							 
  DispRam[40+19] = (i%10)+'0';
}
*/

void DispState3(void)
{
  DispRam[20] = (LiftRunFloor[0]/10)+'0';
  DispRam[21] = (LiftRunFloor[0]%10)+'0';							 
  DispRam[22] = (LiftEnDecFloor[0]/10)+'0';
  DispRam[23] = (LiftEnDecFloor[0]%10)+'0';							 

  DispRam[25] = (LiftRunFloor[1]/10)+'0';
  DispRam[26] = (LiftRunFloor[1]%10)+'0';							 
  DispRam[27] = (LiftEnDecFloor[1]/10)+'0';
  DispRam[28] = (LiftEnDecFloor[1]%10)+'0';							 

  DispRam[30] = (LiftRunFloor[2]/10)+'0';
  DispRam[31] = (LiftRunFloor[2]%10)+'0';							 
  DispRam[32] = (LiftEnDecFloor[2]/10)+'0';
  DispRam[33] = (LiftEnDecFloor[2]%10)+'0';							 

  DispRam[35] = (LiftRunFloor[3]/10)+'0';
  DispRam[36] = (LiftRunFloor[3]%10)+'0';							 
  DispRam[37] = (LiftEnDecFloor[3]/10)+'0';
  DispRam[38] = (LiftEnDecFloor[3]%10)+'0';							 

  DispRam[40] = (LiftRunFloor[4]/10)+'0';
  DispRam[41] = (LiftRunFloor[4]%10)+'0';							 
  DispRam[42] = (LiftEnDecFloor[4]/10)+'0';
  DispRam[43] = (LiftEnDecFloor[4]%10)+'0';							 

  DispRam[45] = (LiftRunFloor[5]/10)+'0';
  DispRam[46] = (LiftRunFloor[5]%10)+'0';							 
  DispRam[47] = (LiftEnDecFloor[5]/10)+'0';
  DispRam[48] = (LiftEnDecFloor[5]%10)+'0';							 

  DispRam[50] = (LiftRunFloor[6]/10)+'0';
  DispRam[51] = (LiftRunFloor[6]%10)+'0';						 
  DispRam[52] = (LiftEnDecFloor[6]/10)+'0';
  DispRam[53] = (LiftEnDecFloor[6]%10)+'0';							 

  DispRam[55] = (LiftRunFloor[7]/10)+'0';
  DispRam[56] = (LiftRunFloor[7]%10)+'0';							 
  DispRam[57] = (LiftEnDecFloor[7]/10)+'0';
  DispRam[58] = (LiftEnDecFloor[7]%10)+'0';							 
}
/*--------------------------------------------------------------------------------------------------*/
void MenuChar(DWORD *MData, DWORD *MRam, DWORD Max, DWORD Min, const BYTE *PData, DWORD M )
{
  if(Modify == 0) { //显示实际的数据
     copy(MRam, &PData[(*MData)*M], M); 
  }               
  else {                                                           //修改数据
    if(ModifyID == 0) { ModifyData = *MData; ModifyID = 1;}        //读数据到临时变量 
    if(NowKeyValue != OldKeyValue) {                               //按钮
       OldKeyValue = NowKeyValue;
	   switch(NowKeyValue)
       {
	     case   KEY_OK: Modify = 0; ModifyID = 0;                   //退出修改
                        *MData = ModifyData;                        //临时变量保存
                        Modified = 1;
                        break;
	     case   KEY_UP: if(ModifyData < Min) { ModifyData = Min; }
                        if(ModifyData < Max) { ModifyData++; }
		                else { ModifyData = Min; }
					    break;
         case KEY_DOWN: if(ModifyData > Max) { ModifyData = Max; } 
		                if(ModifyData > Min) { ModifyData--; }
					    else { ModifyData = Max; }
					    break;
	     case  KEY_ESC: Modify = 0; ModifyID = 0;                   //退出修改
         default:	    break;
       } 
    } 
    if (DispOn500ms > 5) {
	      Clr(MRam, M);
	  }      
    else {
	     copy(MRam, &PData[ModifyData*M], M);
	  }                                           
  }
}
/*--------------------------------------------------------------------------------------------------*/
const DWORD NUM_TAB[]= 
{
  0,1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,
};

void ChangeCharData(DWORD data, DWORD *P, DWORD N)
{
  DWORD i,j;
  j = NUM_TAB[N+1];
  data %= j;
  for(i=N; i>0; i--) {
	 j = NUM_TAB[i];
     *P = ((data)/j); 
	 data %= j;
     P++; 
  }
}

DWORD ChangeLongData(DWORD *P, DWORD N)
{
  DWORD i,j;
  j = 0;
  for(i=N; i>0; i--) {
     j += ((DWORD)(*P))*NUM_TAB[i];
	 P++;
  }
  return(j);
}
//0:位数, 8:点, 16:单位，24:正负
void DispData(DWORD *P, DWORD *Ram_P, DWORD Ram_N)
{
  DWORD i;
  for(i=0; i<(Ram_N&0xff); i++) {
     if(i==((Ram_N>>8)&0xff)) {
	    if((Ram_N&0xff00)==0) { 
	       (*Ram_P) = '0'; Ram_P++;
		}
		(*Ram_P) = '.';
		Ram_P++; 
     }

     if((Modify < 2)||(ModifyID != i)||(DispOn500ms < 5)) { 
	    (*Ram_P) = (*P)+'0'; 
	 }
     else { (*Ram_P) = ' '; }
	 P++;
	 Ram_P++;
  }
  switch ((Ram_N>>16)&0xff)
  {
     case D_Hz:
	    (*Ram_P) = 'H'; Ram_P++; (*Ram_P) = 'z';
		break;
     case D_KHz:
	    (*Ram_P) = 'K'; Ram_P++; (*Ram_P) = 'H'; Ram_P++; (*Ram_P) = 'z';
		break;
     case D_A: 
	    (*Ram_P) = 'A';
		break;
     case D_V:
	    (*Ram_P) = 'V';
		break;
     case D_KW:
	    (*Ram_P) = 'K'; Ram_P++; (*Ram_P) = 'W';
		break;
     case D_H:
	    (*Ram_P) = 'H';
		break;
     case D_Min:
	    (*Ram_P) = 'M'; Ram_P++; (*Ram_P) = 'i'; Ram_P++; (*Ram_P) = 'm';
		break;
     case D_S:
	    (*Ram_P) = 'S';
		break;
     case D_mS:	
	    (*Ram_P) = 'm'; Ram_P++; (*Ram_P) = 'S';
		break;
     case D_BFH: 
	    (*Ram_P) = '%';
		break;
     case D_MS:	
	    (*Ram_P) = 'M'; Ram_P++; (*Ram_P) = '/'; Ram_P++; (*Ram_P) = 'S';
		break;
     case D_M:	
	    (*Ram_P) = 'M';
		break;
     case D_mm:
	    (*Ram_P) = 'm'; Ram_P++; (*Ram_P) = 'm';
		break;
     case D_Du:
	    (*Ram_P) = 'C'; Ram_P++; (*Ram_P) = '`';
		break;
     case D_Jiao:
	    (*Ram_P) = '`';
		break;
     case D_RPM:
	    (*Ram_P) = 'R'; Ram_P++; (*Ram_P) = 'P'; Ram_P++; (*Ram_P) = 'M';
		break;
     case D_JSD:
	    (*Ram_P) = 'M'; Ram_P++; (*Ram_P) = '/'; Ram_P++; (*Ram_P) = 'S'; Ram_P++; (*Ram_P) = '2';
		break;
     case D_PS:
	    (*Ram_P) = 'P'; Ram_P++; (*Ram_P) = '/'; Ram_P++; (*Ram_P) = 'S';
		break;
     case D_PPR:
	    (*Ram_P) = 'P'; Ram_P++; (*Ram_P) = 'P'; Ram_P++; (*Ram_P) = 'R';
		break;
	default:
		break;
  }
}
/*--------------------------------------------------------------------------------------------------*/
//显示处理 //(Ram_N 0:位数, 8:点, 16:单位)
void MenuData(DWORD *MData, DWORD *MRam, DWORD Ram_N, DWORD Pre, DWORD Max, DWORD Min)
{
  if(Modify == 0) {//显示实际的数据
     ChangeCharData((*MData)/Pre,BitDispData,(Ram_N&0xff));
  }
  else if(Modify == 1) {//读数据到临时变量
	 Modify = 2; 	  
     ChangeCharData((*MData)/Pre,BitDispData,(Ram_N&0xff));
	 ModifyID = 0;
  }           
  else {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: ModifyID++;
                           if(ModifyID >= (Ram_N&0xff)) { //移出最后一位，退出修改
                              ModifyID = 0;     
                              Modify = 0;
							  *MData = ChangeLongData(BitDispData,(Ram_N&0xff))*Pre; 
							  Modified = 1;
                           }
                           break;
	        case   KEY_UP: if(BitDispData[ModifyID] < 9) { 
			                  BitDispData[ModifyID]++; 
						   }
						   else { BitDispData[ModifyID] = 0; }  
                           break;
            case KEY_DOWN: if(BitDispData[ModifyID] > 0) { 
			                  BitDispData[ModifyID]--; 
						   }
						   else { BitDispData[ModifyID] = 9; }  
                           break;
	        case  KEY_ESC: ModifyID = 0; 
			               Modify = 0;        //退出修改
            default:	   break;
        }
		if(ChangeLongData(BitDispData,(Ram_N&0xff))>Max) { ChangeCharData(Max,BitDispData,(Ram_N&0xff)); }
		else if(ChangeLongData(BitDispData,(Ram_N&0xff))<Min) { ChangeCharData(Min,BitDispData,(Ram_N&0xff)); }
     }
  }
  DispData(BitDispData, MRam, Ram_N); 
}
/*--------------------------------------------------------------------------------------------------*/
//显示处理 //(Ram_N 0:位数, 8:点, 16:单位)
void GroupMenuData(DWORD *MData, DWORD *MRam, DWORD Ram_N, DWORD Pre, DWORD Max, DWORD Min, DWORD Gn)
{
  if(Modify == 0) {//显示实际的数据
     ChangeCharData(MData[ModifyData]/Pre,BitDispData,(Ram_N&0xff));
  }
  else if(Modify == 1) {//读数据到临时变量
     if(NowKeyValue != OldKeyValue) {
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: ModifyID = 0;     
                           if((PasswordOK[0]==1)||(PasswordOK[1]==1)) { 
						      Modify = 2; 
							  ModifyID = 0;
						   }
						   break;
	        case   KEY_UP: if(ModifyData < Gn) { ModifyData++; }
						   else { ModifyData = 0; }  
                           break;
            case KEY_DOWN: if(ModifyData > 0) { ModifyData--; }
						   else { ModifyData = Gn; }  
                           break;
	        case  KEY_ESC: ModifyID = 0; 
			               Modify = 0;        //退出修改
            default:	   break;
        }
     }
	 ChangeCharData(MData[ModifyData]/Pre,BitDispData,(Ram_N&0xff));
  }           
  else {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: ModifyID++;
                           if(ModifyID >= (Ram_N&0xff)) { //移出最后一位，退出修改
                              ModifyID = 0;     
                              Modify = 0;
							  MData[ModifyData] = ChangeLongData(BitDispData,(Ram_N&0xff))*Pre; 
							  Modified = 1;
                           }
                           break;
	        case   KEY_UP: if(BitDispData[ModifyID] < 9) { 
			                  BitDispData[ModifyID]++; 
						   }
						   else { BitDispData[ModifyID] = 0; }  
                           break;
            case KEY_DOWN: if(BitDispData[ModifyID] > 0) { 
			                  BitDispData[ModifyID]--; 
						   }
						   else { BitDispData[ModifyID] = 9; }  
                           break;
	        case  KEY_ESC: ModifyID = 0; 
			               Modify = 0;        //退出修改
            default:	   break;
        }
		if(ChangeLongData(BitDispData,(Ram_N&0xff))>Max) { ChangeCharData(Max,BitDispData,(Ram_N&0xff)); }
		else if(ChangeLongData(BitDispData,(Ram_N&0xff))<Min) { ChangeCharData(Min,BitDispData,(Ram_N&0xff)); }
     }
  }
  MRam[0] = 'G';
  MRam[1] = ':';
  if((Modify != 1)||(DispOn500ms < 5)) {
     MRam[2] = ModifyData + '1'; 
  }
  else { MRam[2] = ' '; }
  
  MRam[4] = 'D';
  MRam[5] = ':';
  DispData(BitDispData, &MRam[6], Ram_N); 
}

void GroupMenuData_Byte(BYTE *MData, DWORD *MRam, DWORD Ram_N, DWORD Pre, DWORD Max, DWORD Min, DWORD Gn)
{
  if(Modify == 0) {//显示实际的数据
     ChangeCharData(MData[ModifyData]/Pre,BitDispData,(Ram_N&0xff));
  }
  else if(Modify == 1) {//读数据到临时变量
     if(NowKeyValue != OldKeyValue) {
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: ModifyID = 0;     
                           if((PasswordOK[0]==1)||(PasswordOK[1]==1)) { 
						      Modify = 2; 
							  ModifyID = 0;
						   }
						   break;
	        case   KEY_UP: if(ModifyData < Gn) { ModifyData++; }
						   else { ModifyData = 0; }  
                           break;
            case KEY_DOWN: if(ModifyData > 0) { ModifyData--; }
						   else { ModifyData = Gn; }  
                           break;
	        case  KEY_ESC: ModifyID = 0; 
			               Modify = 0;        //退出修改
            default:	   break;
        }
     }
	 ChangeCharData(MData[ModifyData]/Pre,BitDispData,(Ram_N&0xff));
  }           
  else {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: ModifyID++;
                           if(ModifyID >= (Ram_N&0xff)) { //移出最后一位，退出修改
                              ModifyID = 0;     
                              Modify = 1;
							  MData[ModifyData] = ChangeLongData(BitDispData,(Ram_N&0xff))*Pre; 
							  Modified = 1;
                           }
                           break;
	        case   KEY_UP: if(BitDispData[ModifyID] < 9) { 
			                  BitDispData[ModifyID]++; 
						   }
						   else { BitDispData[ModifyID] = 0; }  
                           break;
            case KEY_DOWN: if(BitDispData[ModifyID] > 0) { 
			                  BitDispData[ModifyID]--; 
						   }
						   else { BitDispData[ModifyID] = 9; }  
                           break;
	        case  KEY_ESC: ModifyID = 0; 
			               Modify = 1;        //退出修改
            default:	   break;
        }
		if(ChangeLongData(BitDispData,(Ram_N&0xff))>Max) { ChangeCharData(Max,BitDispData,(Ram_N&0xff)); }
		else if(ChangeLongData(BitDispData,(Ram_N&0xff))<Min) { ChangeCharData(Min,BitDispData,(Ram_N&0xff)); }
     }
  }
  MRam[0] = 'G';
  MRam[1] = ':';
  if((Modify != 1)||(DispOn500ms < 5)) {
     MRam[2] = ModifyData + '1'; 
  }
  else { MRam[2] = ' '; }
  
  MRam[4] = 'D';
  MRam[5] = ':';
  DispData(BitDispData, &MRam[6], Ram_N); 
}
/*--------------------------------------------------------------------------------------------------*/
//显示年月日，时分秒
void DispData2For3(DWORD data1, DWORD data2, DWORD data3, DWORD *Ram_P, DWORD DT)
{
  if(DT > 0) {//时间 
     Ram_P -= 2;
	 Ram_P[4] =	':';
	 Ram_P[7] = ':';  
  }
  else {
	 Ram_P[4] =	'/';
	 Ram_P[7] = '/';  
  }

  if((Modify == 0)||(ModifyID != 0)||(DispOn500ms < 5)) {
    if(DT == 0) {
       Ram_P[0] = (data1/1000+0x30); 
       data1 = data1%1000;
       Ram_P[1] = (data1/100+0x30); 
       data1 = data1%100;
	}
    Ram_P[2] = (data1/10+0x30); 
    data1 = data1%10;
    Ram_P[3] = (data1+0x30); 
  }
  else { 
    if(DT == 0) {
       Ram_P[0] = ' '; 
	   Ram_P[1] = ' ';
    }
	Ram_P[2] = ' ';
	Ram_P[3] = ' ';
  }
  //-------->> 
  if((Modify == 0)||(ModifyID != 1)||(DispOn500ms < 5)) {
     Ram_P[5] = (data2/10+0x30); 
     data2 = data2%10;
     Ram_P[6] = (data2+0x30); 
  }
  else { 
     Ram_P[5] = ' '; 
	   Ram_P[6] = ' ';
  }
  //-------->> 
  if((Modify == 0)||(ModifyID != 2)||(DispOn500ms < 5)) {
     Ram_P[8] = (data3/10+0x30); 
     data3 = data3%10;
     Ram_P[9] = (data3+0x30); 
  }
  else { 
     Ram_P[8] = ' '; 
	   Ram_P[9] = ' ';
  }
  //-------->> 
}
/*--------------------------------------------------------------------------------------------------*/
const DWORD RiQiMax[2][3] = {
   {2099,12,31},
   {23,59,59},
};
const DWORD RiQiMin[2][3] = {
   {2000,1,1},
   {0,0,0},
};
//年月日(DT>0),时分秒(DT == 0)
void MenuData2For3(volatile DWORD *MData1, volatile DWORD *MData2, volatile DWORD *MData3, DWORD *MRam, DWORD DT)
{
  if(Modify == 0) {//显示实际的数据
     DispData2For3(*MData1, *MData2, *MData3, MRam, DT); 
	   return;
  } 
  else if(Modify == 1) { //读数据到临时变量 
     Modify = 2;
	   ModifyData = *MData1; 
	   ModifyID = 0;      
  }
  else {
     if(NowKeyValue != OldKeyValue) {//按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: Modified = 1;
			               if(ModifyID == 0) { 
			                  *MData1 = ModifyData;	//保存临时变量
							  ModifyData = *MData2; //读数据到临时变量
						   }           
                           else if(ModifyID == 1) { 
						      *MData2 = ModifyData;	//保存临时变量
							  ModifyData = *MData3; //读数据到临时变量 
						   }      
                           else {             
						      *MData3 = ModifyData;	//保存临时变量
							  Modify = 0;  			//退出修改
							  ModifyID = 0; 
							  break; 
						   }
                           ModifyID++;
                           break;
	        case   KEY_UP: if(ModifyData < RiQiMax[DT][ModifyID]) { ModifyData++; }
			               else { ModifyData = RiQiMin[DT][ModifyID]; }
                           break;
            case KEY_DOWN: if(ModifyData > RiQiMin[DT][ModifyID]) { ModifyData--; }
						   else { ModifyData = RiQiMax[DT][ModifyID]; }
						   break;
	        case  KEY_ESC: Modify = 0; ModifyID = 0;                            //退出修改
            default:	   break;
        }
     }
  }
  //显示临时数据
  if(ModifyID == 0) { 
     DispData2For3(ModifyData, *MData2, *MData3, MRam, DT);  
  }
  else if(ModifyID == 1) { 
     DispData2For3(*MData1, ModifyData, *MData3, MRam, DT); 
  }
  else { 
     DispData2For3(*MData1, *MData2, ModifyData, MRam, DT); 
  }
}

/*--------------------------------------------------------------------------------------------------*/

void MenuSetTime( DWORD *T)  
{ 
   DWORD En_H,En_M,En_S;
   En_H = ((*T)>>16)&0xff;
   En_M = ((*T)>>8)&0xff;
   En_S = (*T)&0xff;
   MenuData2For3(&En_H, &En_M, &En_S, &DispRam[25], 1);
   (*T) = (En_H<<16)|(En_M<<8)|En_S; 
}
/*--------------------------------------------------------------------------------------------------*/
//显示 总启动使能 时 分 星期使能
void EnOrderDisp(DWORD Data, DWORD *Ram_P)
{
	DWORD i; 
  if ((Modify == 0)||(ModifyID != 0)||(DispOn500ms < 5)) {
      if((Data&0x01) == 0) {
          Ram_P[0] = 'O'; 
          Ram_P[1] = 'F';
          Ram_P[2] = 'F'; 
	    }
			else {
          Ram_P[0] = 'O'; 
          Ram_P[1] = 'N';
          Ram_P[2] = ' '; 
			}
  }
  else { 
      Ram_P[0] = ' '; 
      Ram_P[1] = ' ';
      Ram_P[2] = ' '; 
  }
	
  //-------->> hour
  if((Modify == 0)||(ModifyID != 1)||(DispOn500ms < 5)) {
		  i = (Data>>18)&0x1f;
      Ram_P[4] = (i/10+0x30); 
      Ram_P[5] = (i%10+0x30); 
  }
  else { 
      Ram_P[4] = ' '; 
	    Ram_P[5] = ' ';
  }
	Ram_P[6] =	':';
  //-------->> min
  if((Modify == 0)||(ModifyID != 2)||(DispOn500ms < 5)) {
		  i = (Data>>12)&0x3f;
      Ram_P[7] = (i/10+0x30); 
      Ram_P[8] = (i%10+0x30); 
  }
  else { 
      Ram_P[7] = ' '; 
	    Ram_P[8] = ' ';
  }
	
	Ram_P[9] =	'-';
	Ram_P[10] =	'>';
		
  //-------->> hour
  if((Modify == 0)||(ModifyID != 3)||(DispOn500ms < 5)) {
		  i = (Data>>7)&0x1f;
      Ram_P[11] = (i/10+0x30); 
      Ram_P[12] = (i%10+0x30); 
  }
  else { 
      Ram_P[11] = ' '; 
	    Ram_P[12] = ' ';
  }
	Ram_P[13] =	':';
  //-------->> min
  if((Modify == 0)||(ModifyID != 4)||(DispOn500ms < 5)) {
		  i = (Data>>1)&0x3f;
      Ram_P[14] = (i/10+0x30); 
      Ram_P[15] = (i%10+0x30); 
  }
  else { 
      Ram_P[14] = ' '; 
	    Ram_P[15] = ' ';
  }

  //-------->> week
  Ram_P[20] =	'w';
	Ram_P[21] =	'e';
	Ram_P[22] =	'e';
	Ram_P[23] =	'k';
	Ram_P[24] =	':';
  for (i=0; i<7; i++)
  {
		  if((Modify == 0)||(ModifyID != (5+i))||(DispOn500ms < 5)) {
					if ( Data&(1ul<<(23+i)) ) {
							Ram_P[25+i] = '1';
					} 
					else {
							Ram_P[25+i] = '0';
					}
			}      
			else {
					Ram_P[25+i] = ' ';
			}        	
	}	
}
/*--------------------------------------------------------------------------------------------------*/
const DWORD EnMaskBit[12] = {
   (1ul<<0),(0x1ful<<18),(0x3ful<<12),(0x1ful<<7),(0x3ful<<1),(1ul<<23),(1ul<<24),(1ul<<25),(1ul<<26),(1ul<<27),(1ul<<28),(1ul<<29)
};
const DWORD EnMax[12] = {
   (1ul<<0),  (23ul<<18),  (59ul<<12),  (23ul<<7),  (59ul<<1),(1ul<<23),(1ul<<24),(1ul<<25),(1ul<<26),(1ul<<27),(1ul<<28),(1ul<<29)
};
const DWORD EnAddOne[12] = {
   (1ul<<0),   (1ul<<18),   (1ul<<12),   (1ul<<7),   (1ul<<1),(1ul<<23),(1ul<<24),(1ul<<25),(1ul<<26),(1ul<<27),(1ul<<28),(1ul<<29)
};

void MenuEnOrder(DWORD *MData, DWORD *MRam)
{	
  if (Modify == 0) {//显示实际的数据
      ModifyData = *MData;		
  } 
  else if(Modify == 1) { //读数据到临时变量 
      Modify = 2;
	    ModifyData = *MData; 
	    ModifyID = 0;      
  }
  else {
      if (NowKeyValue != OldKeyValue) {//按钮
         OldKeyValue = NowKeyValue;
         switch(NowKeyValue)
         {
	        case   KEY_OK: ModifyID++;
												 if (ModifyID>11) {
												    *MData = ModifyData;	//保存临时变量
							              Modify = 0;  			//退出修改
							              ModifyID = 0; 
							              Modified = 1;
			                   }
                         break;
	        case   KEY_UP: if ( (ModifyData&EnMaskBit[ModifyID]) < EnMax[ModifyID] ) { ModifyData += EnAddOne[ModifyID]; }
			                   break;
          case KEY_DOWN: if ( (ModifyData&EnMaskBit[ModifyID]) > 0 ) { ModifyData -= EnAddOne[ModifyID]; }
						             break;
	        case  KEY_ESC: Modify = 0; ModifyID = 0;                            //退出修改
            default:	   break;
         }
      }
  }
  //显示数据
  EnOrderDisp(ModifyData, MRam);
}

/*--------------------------------------------------------------------------------------------------*/
//显示位字符
void DispCallData(DWORD *BitData)
{
   DWORD i,j;
   DispRam[20] = '(';
   if((DispOn500ms > 5)&&(Modify == 1)) {
       DispRam[21] = ' ';
	     DispRam[22] = ' ';
   }
   else {
       DispRam[21] = ((ModifyID+1)/10)+'0';
	     DispRam[22] = ((ModifyID+1)%10)+'0';
   }
   DispRam[23] = ')';

   for(i=0; i<64; i++) {
       if(ModifyID<56) {
	         if(i<56)  { j = i+24; }
	         else { break; } 
	     }
	     else {
		       if(i<56) { DispRam[i+24+8] = ' '; continue; }
		       else { j = i-56+24; }
	     }
       if((i == ModifyID)&&(Modify > 0)) {
					 if(Modify == 1) { 
							if(DispOn500ms > 5) { DispRam[j] = ' '; }
							else {//显示临时数据
									if(Bit64(BitData, i, 2)) { DispRam[j] = '1'; }
									else { DispRam[j] = '0'; }
							} 
					 }
					 else if(Modify == 2) { 
							if(DispOn500ms > 5) { DispRam[j] = 0xff; }
							else { DispRam[j] = ModifyData+'0'; } //显示临时数据
					 }
	     }
	     else { //显示实际的数据
		       if(Bit64(BitData, i, 2)) { DispRam[j] = '1'; }
		       else { DispRam[j] = '0'; }
	     }
   }
}
/*--------------------------------------------------------------------------------------------------*/

void MenuCallBit(DWORD *BitData)
{
  if(ModifyID > 63) { ModifyID = 0; }
  if(Modify == 1) {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: if(Bit64(BitData, ModifyID, 2)) { ModifyData = 1; }
		                   else { ModifyData = 0; }
		                   Modify = 2; 
                           break;
	        case   KEY_UP: if(ModifyID > 62) { ModifyID = 0; }
                           else { ModifyID ++; }
                           break;
            case KEY_DOWN: if(ModifyID == 0) { ModifyID = 63; }
                           else { ModifyID--; }
                           break;
	        case  KEY_ESC: ModifyID = 0;
			               Modify = 0; //退出修改                   
            default:	   break;
        }
     }
  }
  else  {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: //保存临时变量
			               if(ModifyData) { Bit64(BitData, ModifyID, 1); }
						   else { Bit64(BitData, ModifyID, 0); }
			               Modified = 1;
                           Modify = 1;
                           break;
	        case   KEY_UP: 
            case KEY_DOWN: if(ModifyData) { ModifyData = 0; }
                           else { ModifyData=1; }
                           break;
	        case  KEY_ESC: Modify = 1;  //退出修改
            default:	   break;
        }
     }
  }
  DispCallData(BitData);
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
//显示位字符
void DispCallDataTwo(DWORD *BitData, DWORD *BitDataB)
{
   DWORD i,j,k;
   DispRam[20] = '(';
   if((DispOn500ms > 5)&&(Modify == 1)) {
       DispRam[21] = ' ';
	     DispRam[22] = ' ';
   }
   else {
       DispRam[21] = ((ModifyID+1)/10)+'0';
	     DispRam[22] = ((ModifyID+1)%10)+'0';
   }
   DispRam[23] = ')';

   for(i=0; i<64; i++) {
       if(ModifyID<56) {
	         if(i<56)  { j = i+24; }
	         else { break; } 
	     }
	     else {
		       if(i<56) { DispRam[i+24+8] = ' '; continue; }
		       else { j = i-56+24; }
	     }
			 
			 k = 0;
			 if(Bit64(BitData, i, 2)) { k |= 1; }
			 if(Bit64(BitDataB, i, 2)) { k |= 2; }
			             
       if((i == ModifyID)&&(Modify > 0)) {
	         if(Modify == 1) { 
	             if(DispOn500ms > 5) { DispRam[j] = ' '; }
		           else {//显示临时数据
								   DispRam[j] = '0'+k;
	             } 
	         }
	         else if(Modify == 2) { 
	             if(DispOn500ms > 5) { DispRam[j] = 0xff; }
		           else { DispRam[j] = ModifyData+'0'; } //显示临时数据
	         }
	     }
	     else { //显示实际的数据
		       DispRam[j] = '0'+k;
	     }
   }
}
/*--------------------------------------------------------------------------------------------------*/

void MenuCallBitTwo(DWORD *BitData, DWORD *BitDataB)
{
  if(ModifyID > 63) { ModifyID = 0; }
  if(Modify == 1) {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: ModifyData = 0;
                         if(Bit64(BitData, ModifyID, 2)) { ModifyData |= 1; }
		                     if(Bit64(BitDataB, ModifyID, 2)) { ModifyData |= 2; }
		                     Modify = 2; 
                         break;
	        case   KEY_UP: if(ModifyID > 62) { ModifyID = 0; }
                         else { ModifyID ++; }
                         break;
          case KEY_DOWN: if(ModifyID == 0) { ModifyID = 63; }
                         else { ModifyID--; }
                         break;
	        case  KEY_ESC: ModifyID = 0;
			                   Modify = 0; //退出修改                   
            default:	   break;
        }
     }
  }
  else  {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: //保存临时变量
			                   if(ModifyData&0x01) { Bit64(BitData, ModifyID, 1); }
						             else                { Bit64(BitData, ModifyID, 0); }
			                   if(ModifyData&0x02) { Bit64(BitDataB, ModifyID, 1); }
						             else                { Bit64(BitDataB, ModifyID, 0); }
			                   Modified = 1;
                         Modify = 1;
                         break;
	        case   KEY_UP: ModifyData = (ModifyData+1)&0x3;
												 break;
          case KEY_DOWN: ModifyData = (ModifyData-1)&0x3;
                         break;
	        case  KEY_ESC: Modify = 1;  //退出修改
            default:	   break;
        }
     }
  }
  DispCallDataTwo(BitData, BitDataB);
}
/*--------------------------------------------------------------------------------------------------*/
//显示位字符
void DispBit32(DWORD *BitData)
{
   DWORD i;
   DispRam[28] = '(';
   if((DispOn500ms > 5)&&(Modify == 1)) {
      DispRam[29] = ' ';
	  DispRam[30] = ' ';
   }
   else {
      DispRam[29] = ((ModifyID+1)/10)+'0';
	  DispRam[30] = ((ModifyID+1)%10)+'0';
   }
   DispRam[31] = ')';

   for(i=0; i<32; i++) {
      if((i == ModifyID)&&(Modify > 0)) {
	     if(Modify == 1) { 
	        if(DispOn500ms > 5) { DispRam[i+40] = ' '; }
		    else {//显示临时数据
			   if(Bit64(BitData, i, 2)) { DispRam[i+40] = '1'; }
			   else { DispRam[i+40] = '0'; }
	        } 
	     }
	     else if(Modify == 2) { 
	        if(DispOn500ms > 5) { DispRam[i+40] = 0xff; }
		    else { DispRam[i+40] = ModifyData+'0'; } //显示临时数据
	     }
	  }
	  else { //显示实际的数据
		 if(Bit64(BitData, i, 2)) { DispRam[i+40] = '1'; }
		 else { DispRam[i+40] = '0'; }
	  }
   }
}
/*--------------------------------------------------------------------------------------------------*/

void MenuBit32(DWORD *BitData)
{
  if(ModifyID > 31) { ModifyID = 0; }
  if(Modify == 1) {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: if(Bit64(BitData, ModifyID, 2)) { ModifyData = 1; }
		                   else { ModifyData = 0; }
		                   Modify = 2; 
                           break;
	        case   KEY_UP: if(ModifyID > 30) { ModifyID = 0; }
                           else { ModifyID ++; }
                           break;
            case KEY_DOWN: if(ModifyID == 0) { ModifyID = 31; }
                           else { ModifyID--; }
                           break;
	        case  KEY_ESC: ModifyID = 0;
			               Modify = 0; //退出修改                   
            default:	   break;
        }
     }
  }
  else  {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: //保存临时变量
			               if(ModifyData) { Bit64(BitData, ModifyID, 1); }
						   else { Bit64(BitData, ModifyID, 0); }
			               Modified = 1;
                           Modify = 1;
                           break;
	        case   KEY_UP: 
            case KEY_DOWN: if(ModifyData) { ModifyData = 0; }
                           else { ModifyData=1; }
                           break;
	        case  KEY_ESC: Modify = 1;  //退出修改
            default:	   break;
        }
     }
  }
  DispBit32(BitData);
}
/*--------------------------------------------------------------------------------------------------*/
//显示位字符
void DispUpDnData(DWORD *BitData1, DWORD *BitData2)
{
   DWORD i,j,k;
   for(i=1,j=1; i<61; i++) {
      if(i==33) {
	     BitData1++;
		 BitData2++;
		 j = 1;
	  }
      k = 0;
	  if(((*BitData1)&j)>0) { k |= 0x01; }
	  if(((*BitData2)&j)>0) { k |= 0x02; }
	  if(k==0x01) { DispRam[i+19] = 0; }
	  else if(k==0x02) { DispRam[i+19] = 1; }
	  else if(k==0x03) { DispRam[i+19] = 2; }
	  else { DispRam[i+19] = '-'; }
	  j <<= 1;
   }
}
/*--------------------------------------------------------------------------------------------------*/
void FunPassword(DWORD i) 
{
  if(Modify == 0) {
     copy(&DispRam[20], "      ********      ", 20);
     return; 
  }
  else if(Modify == 1) {
     ModifyData = 1;
     if(PasswordOK[i] == 1) {
	  	if(DispOn500ms > 5) { Clr(&DispRam[20], 20); }
        else { copy(&DispRam[20], "  Modify Password?  ", 20); }
        if(NowKeyValue != OldKeyValue) {                               //按钮
           OldKeyValue = NowKeyValue;
           if(NowKeyValue == KEY_OK) { ModifyData = 0; }
		   else if(NowKeyValue == KEY_ESC) { Modify = 0; }
	    }
     }
	 else { ModifyData = 0; }

     if(ModifyData == 0) { 
		Modify = 2;  
        ModifyID = 0;
		ChangeCharData(0,BitDispData,5);
		Clr(&DispRam[20], 20);
	 }
  } 
  else if(Modify == 2) {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue)
        {
	        case   KEY_OK: ModifyID++;
                           if(ModifyID >= 5) { //移出最后一位，退出修改
						      ModifyData = ChangeLongData(BitDispData,5);
			                  if(PasswordOK[i] == 1) { //修改密码
                                 Password[i] = ModifyData;
                                 Modified = 1;
                                 copy(&DispRam[20], "  Modify Success !  ", 20);
                              }
                              else if(Password[i] == ModifyData) { //密码核查
                                 PasswordOK[i] = 1;
								 if(PasswordOK[1]==1) { PasswordOK[0] = 1; } 
                                 PasswordTime = 0;
                                 copy(&DispRam[20], "   Password OK !    ", 20);
                              }    
                              else { 
                                 copy(&DispRam[20], "  Password Error !  ", 20);
                              }
                              ModifyID = 0;     
                              Modify = 3;
							  return;
						   }
                           break;
	        case   KEY_UP: if(BitDispData[ModifyID] < 9) { 
			                  BitDispData[ModifyID]++; 
						   }
						   else { BitDispData[ModifyID] = 0; }  
                           break;
            case KEY_DOWN: if(BitDispData[ModifyID] > 0) { 
			                  BitDispData[ModifyID]--; 
						   }
						   else { BitDispData[ModifyID] = 9; }  
                           break;
	        case  KEY_ESC: ModifyID = 0; 
			               Modify = 0;        //退出修改
            default:	   break;
        }
     }
	   DispData(BitDispData, &DispRam[25], (0xff<<8)|5);
  }
  else {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        if(NowKeyValue!=KEY_NULL) { Modify = 0;	}
	 }
  }
}
/*--------------------------------------------------------------------------------------------------*/
const BYTE InitPlayTab[][20] = {
   {"(0) 32F 32F 32F     "},
   {"(1) F:MaxFlr G:Addr "},
   {"(2)F:MaxFlr Odd-even"},
   {"(3)MaxFlr Odd-even 2"},
   {"(4)  14F 14F 14F    "},
   {"(5)  16F 16F 16F    "},
   {"(6)  32F 32P 32O    "},
   {"(7)  28F 27P 28O    "},
   {"(8)  28F 27P 28O    "},
   {"(9)  26F 25P 26O    "},
   {"(10) 31F 30P 31O    "},
   {"(11) 24F 23P 24O    "},
   {"(12) 22F 21F 21F    "},
   {"(13) 20F 19O 19P    "},
   {"(14) 13F 12O 13P    "},
   {"(15) 20F 19F 19F    "},
   {"(16) 22F 21F 21F    "},
   {"(17) 8Lift 1F 9F-23F"},
   {"(18) 8Lift 1F 9F-25F"},
   {"(19) 8Lift 1F-18F   "},
   {"(20) 6Lift 1F-10F   "},
	 {"(21) 3Lift 1F 3F-36F"},
	 {"(22) 3Lift 1F 3F-21F"},
	 {"(23) 3Lift 22F      "},
	 
	 {"(24) spare          "},
	 {"(25) spare          "},
	 {"(26) spare          "},
	 {"(27) spare          "},
	 {"(28) spare          "},
	 {"(29) spare          "},
	 {"(30) spare          "},
	 {"(31) spare          "},
	 {"(32) Standard 2 lift"},
	 {"(33) Standard 3 lift"},
	 {"(34) Standard 4 lift"},
	 {"(35) Standard 5 lift"},
	 {"(36) Standard 6 lift"},
	 {"(37) Standard 7 lift"},
	 {"(38) Standard 8 lift"},
	 {"(39) 3Lift 14F      "},
	 {"(40) spare          "},
	 {"(41) spare          "},
	 {"(42) spare          "},
	 {"(43) spare          "},
	 {"(44) spare          "},
	 {"(45) spare          "},
	 {"(46) spare          "},
	 {"(47) spare          "},
	 {"(48) spare          "},
	 {"(49) spare          "},
	 {"(50) spare          "},
	 {"(51) spare          "},
	 {"(52) spare          "},
	 {"(53) spare          "},
	 {"(54) spare          "},
	 {"(55) spare          "},
	 {"(56) spare          "},
	 {"(57) spare          "},
	 {"(58) spare          "},
	 {"(59) spare          "},
	 {"(60) spare          "},	 
};

void FunFactoryPara(void)
{
  if(Modify == 0) { 
     ModifyID = 0;
	 copy(&DispRam[20], "           Enter    ", 20); 
	 return; 
  }
  else if(Modify == 1) { 
     Modify = 2; 
	 ModifyData = 0; 
	 return; 
  } 
  else if(Modify == 2) { //修改数据
     if(DispOn500ms < 5) {
	    copy(&DispRam[20], InitPlayTab[ModifyData], 20);
     }
     else { Clr(&DispRam[20], 20); }  

     if(NowKeyValue != OldKeyValue) {                  //按钮
        OldKeyValue = NowKeyValue;
        switch(NowKeyValue) {
	       case   KEY_OK: ModifyID = 0;                //退出修改
						  InitData(ModifyData);
					    Modified = 1;
						  copy(&DispRam[20], "   Init Success!    ", 20);
						  Modify = 3;  
                          break;
	       case   KEY_UP: if(ModifyData<60) { ModifyData++; }
		                  else                { ModifyData=0; } 
                          break;
           case KEY_DOWN: if(ModifyData>0){ ModifyData--; }
		                  else                { ModifyData=60; } 
                          break;
	       case  KEY_ESC: Modify = 0; ModifyID = 0;                   //退出修改
                          copy(&DispRam[20], "           Enter    ", 20);
           default:	      break;
        } 
     } 
  }
  else {
     if(NowKeyValue != OldKeyValue) {                               //按钮
        OldKeyValue = NowKeyValue;
        if(NowKeyValue!=KEY_NULL) { Modify = 0;	}
	 }
  }
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/




