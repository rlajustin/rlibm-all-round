#ifndef _INTERVAL_H
#define _INTERVAL_H

#include <math.h>
#include <mpfr.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "helper.h"

#define LN2HIGH 0.69314718055994528622676398299518041312694549560546875

typedef struct IInterval{
        double lo;
        double hi;
} Interval;

double oneByF[128] = {
    1.000000000000000000000000000000000000000000000000000000000000,
    0.992248062015503862198784190695732831954956054687500000000000,
    0.984615384615384670041748904623091220855712890625000000000000,
    0.977099236641221335908369383105309680104255676269531250000000,
    0.969696969696969723884194536367431282997131347656250000000000,
    0.962406015037593931538140168413519859313964843750000000000000,
    0.955223880597014907145592133019817993044853210449218750000000,
    0.948148148148148184333194876671768724918365478515625000000000,
    0.941176470588235281056199710292275995016098022460937500000000,
    0.934306569343065662636149681929964572191238403320312500000000,
    0.927536231884057982277624887501588091254234313964843750000000,
    0.920863309352518033534806818352080881595611572265625000000000,
    0.914285714285714257165693652495974674820899963378906250000000,
    0.907801418439716290009755539358593523502349853515625000000000,
    0.901408450704225372440703267784556373953819274902343750000000,
    0.895104895104895104118725157604785636067390441894531250000000,
    0.888888888888888839545643349993042647838592529296875000000000,
    0.882758620689655160163056279998272657394409179687500000000000,
    0.876712328767123239003922208212316036224365234375000000000000,
    0.870748299319727858680550980352563783526420593261718750000000,
    0.864864864864864912874509172979742288589477539062500000000000,
    0.859060402684563739761358647228917106986045837402343750000000,
    0.853333333333333388104335881507722660899162292480468750000000,
    0.847682119205298012509786076407181099057197570800781250000000,
    0.842105263157894690095872647361829876899719238281250000000000,
    0.836601307189542509057389452209463343024253845214843750000000,
    0.831168831168831223621396020462270826101303100585937500000000,
    0.825806451612903225090178693790221586823463439941406250000000,
    0.820512820512820484353255778842139989137649536132812500000000,
    0.815286624203821697065563967043999582529067993164062500000000,
    0.810126582278481000010117440979229286313056945800781250000000,
    0.805031446540880546436369513685349375009536743164062500000000,
    0.800000000000000044408920985006261616945266723632812500000000,
    0.795031055900621064225219924992416054010391235351562500000000,
    0.790123456790123412929460755549371242523193359375000000000000,
    0.785276073619631920230688137962715700268745422363281250000000,
    0.780487804878048807566415234759915620088577270507812500000000,
    0.775757575757575756902895136590814217925071716308593750000000,
    0.771084337349397630489988841873127967119216918945312500000000,
    0.766467065868263519590186660934705287218093872070312500000000,
    0.761904761904761862467694299994036555290222167968750000000000,
    0.757396449704141994097028600663179531693458557128906250000000,
    0.752941176470588224844959768233820796012878417968750000000000,
    0.748538011695906391196331242099404335021972656250000000000000,
    0.744186046511627896649088143021799623966217041015625000000000,
    0.739884393063583778449299188650911673903465270996093750000000,
    0.735632183908045966802546899998560547828674316406250000000000,
    0.731428571428571427937015414499910548329353332519531250000000,
    0.727272727272727292913145902275573462247848510742187500000000,
    0.723163841807909602010795424575917422771453857421875000000000,
    0.719101123595505598018462478648871183395385742187500000000000,
    0.715083798882681587194554140296531841158866882324218750000000,
    0.711111111111111138249896157503826543688774108886718750000000,
    0.707182320441988920833864540327340364456176757812500000000000,
    0.703296703296703351604435283661587163805961608886718750000000,
    0.699453551912568305404249713319586589932441711425781250000000,
    0.695652173913043458952643049997277557849884033203125000000000,
    0.691891891891891930299607338383793830871582031250000000000000,
    0.688172043010752743086300142749678343534469604492187500000000,
    0.684491978609625628671153663162840530276298522949218750000000,
    0.680851063829787217507316654518945142626762390136718750000000,
    0.677248677248677211082394933328032493591308593750000000000000,
    0.673684210526315774281158610392594709992408752441406250000000,
    0.670157068062827265819692001969087868928909301757812500000000,
    0.666666666666666629659232512494781985878944396972656250000000,
    0.663212435233160646497196921700378879904747009277343750000000,
    0.659793814432989678131491473322967067360877990722656250000000,
    0.656410256410256409687065115576842799782752990722656250000000,
    0.653061224489795866254837619635509327054023742675781250000000,
    0.649746192893400964507577555195894092321395874023437500000000,
    0.646464646464646519596897178416838869452476501464843750000000,
    0.643216080402010059735573577199829742312431335449218750000000,
    0.640000000000000013322676295501878485083580017089843750000000,
    0.636815920398009938097061422013211995363235473632812500000000,
    0.633663366336633671060951655817916616797447204589843750000000,
    0.630541871921182273119654837501002475619316101074218750000000,
    0.627450980392156854037466473528183996677398681640625000000000,
    0.624390243902439023848671695304801687598228454589843750000000,
    0.621359223300970819892086183244828134775161743164062500000000,
    0.618357487922705284510982437495840713381767272949218750000000,
    0.615384615384615418776093065389432013034820556640625000000000,
    0.612440191387559784708116694673663005232810974121093750000000,
    0.609523809523809578791997410007752478122711181640625000000000,
    0.606635071090047439668069273466244339942932128906250000000000,
    0.603773584905660354316125904006185010075569152832031250000000,
    0.600938967136150248293802178523037582635879516601562500000000,
    0.598130841121495282486364430951653048396110534667968750000000,
    0.595348837209302317319270514417439699172973632812500000000000,
    0.592592592592592559697095566662028431892395019531250000000000,
    0.589861751152073732207270495564443990588188171386718750000000,
    0.587155963302752326171685126610100269317626953125000000000000,
    0.584474885844748826002614805474877357482910156250000000000000,
    0.581818181818181789921595736814197152853012084960937500000000,
    0.579185520361991001969670378457522019743919372558593750000000,
    0.576576576576576571575571961147943511605262756347656250000000,
    0.573991031390134520684398466983111575245857238769531250000000,
    0.571428571428571396850770724995527416467666625976562500000000,
    0.568888888888888888395456433499930426478385925292968750000000,
    0.566371681415929195679836993804201483726501464843750000000000,
    0.563876651982378906957649178366409614682197570800781250000000,
    0.561403508771929793397248431574553251266479492187500000000000,
    0.558951965065502154317300664843060076236724853515625000000000,
    0.556521739130434789366574932500952854752540588378906250000000,
    0.554112554112554112073496526136295869946479797363281250000000,
    0.551724137931034475101910174998920410871505737304687500000000,
    0.549356223175965663330089228111319243907928466796875000000000,
    0.547008547008547063583705494238529354333877563476562500000000,
    0.544680851063829796210313816118286922574043273925781250000000,
    0.542372881355932201508096568431938067078590393066406250000000,
    0.540084388185653962999310806480934843420982360839843750000000,
    0.537815126050420144743213768379064276814460754394531250000000,
    0.535564853556485309660217808414017781615257263183593750000000,
    0.533333333333333325931846502498956397175788879394531250000000,
    0.531120331950207469340341503993840888142585754394531250000000,
    0.528925619834710758482287928927689790725708007812500000000000,
    0.526748971193415682634508812043350189924240112304687500000000,
    0.524590163934426256808762900618603453040122985839843750000000,
    0.522448979591836781821712065720930695533752441406250000000000,
    0.520325203252032575385044310678495094180107116699218750000000,
    0.518218623481781381912014694535173475742340087890625000000000,
    0.516129032258064501803573875804431736469268798828125000000000,
    0.514056224899598346311790919571649283170700073242187500000000,
    0.512000000000000010658141036401502788066864013671875000000000,
    0.509960159362549791950414146413095295429229736328125000000000,
    0.507936507936507908311796199996024370193481445312500000000000,
    0.505928853754940677234230861358810216188430786132812500000000,
    0.503937007874015741037965199211612343788146972656250000000000,
    0.501960784313725483229973178822547197341918945312500000000000
};

double lnF[128] = {
    0.0000000000000000000000000000000000000000000000000000000000000000000000e+00,
    7.7821404420549480929203411960770608857274055480957031250000000000000000e-03,
    1.5504186535965252743962672354882670333608984947204589843750000000000000e-02,
    2.3167059281534375936306702215006225742399692535400390625000000000000000e-02,
    3.0771658666753687327855004696175456047058105468750000000000000000000000e-02,
    3.8318864302136594612857578567854943685233592987060546875000000000000000e-02,
    4.5809536031294201263719401140406262129545211791992187500000000000000000e-02,
    5.3244514518812277592552106852963333949446678161621093750000000000000000e-02,
    6.0624621816434839938203538167726946994662284851074218750000000000000000e-02,
    6.7950661908507736796991594019345939159393310546875000000000000000000000e-02,
    7.5223421237587517751421728462446480989456176757812500000000000000000000e-02,
    8.2443669211074585567722294854320352897047996520996093750000000000000000e-02,
    8.9612158689687124168976595228741643950343132019042968750000000000000000e-02,
    9.6729626458551098977522997302003204822540283203125000000000000000000000e-02,
    1.0379679368164355934833764649738441221415996551513671875000000000000000e-01,
    1.1081436634029011301105782649756292812526226043701171875000000000000000e-01,
    1.1778303565638344185817487641543266363441944122314453125000000000000000e-01,
    1.2470347850095722663787967121606925502419471740722656250000000000000000e-01,
    1.3157635778871926146571524895989568904042243957519531250000000000000000e-01,
    1.3840232285911913123754857224412262439727783203125000000000000000000000e-01,
    1.4518200984449788903951628071808954700827598571777343750000000000000000e-01,
    1.5191604202584196858794030049466527998447418212890625000000000000000000e-01,
    1.5860503017663857283636730244325008243322372436523437500000000000000000e-01,
    1.6524957289530714521497145597095368430018424987792968750000000000000000e-01,
    1.7185025692665920060697715143760433420538902282714843750000000000000000e-01,
    1.7840765747281828179637841458315961062908172607421875000000000000000000e-01,
    1.8492233849401198964024217730184318497776985168457031250000000000000000e-01,
    1.9139485299962943898322009772527962923049926757812500000000000000000000e-01,
    1.9782574332991986754137769821682013571262359619140625000000000000000000e-01,
    2.0421554142869088876999228432396193966269493103027343750000000000000000e-01,
    2.1056476910734961416338251183333341032266616821289062500000000000000000e-01,
    2.1687393830061435506806333251006435602903366088867187500000000000000000e-01,
    2.2314355131420973710199007200571941211819648742675781250000000000000000e-01,
    2.2937410106484582006380890106811420992016792297363281250000000000000000e-01,
    2.3556607131276688371634975283086532726883888244628906250000000000000000e-01,
    2.4171993688714515924331749374687206000089645385742187500000000000000000e-01,
    2.4783616390458124145723672882013488560914993286132812500000000000000000e-01,
    2.5391520998096339667426946107298135757446289062500000000000000000000000e-01,
    2.5995752443692604627401010475296061486005783081054687500000000000000000e-01,
    2.6596354849713793599974565040611196309328079223632812500000000000000000e-01,
    2.7193371548364175804834985683555714786052703857421875000000000000000000e-01,
    2.7786845100345625159121709657483734190464019775390625000000000000000000e-01,
    2.8376817313064456316240580235898960381746292114257812500000000000000000e-01,
    2.8963329258304265634293983566749375313520431518554687500000000000000000e-01,
    2.9546421289383584252163927885703742504119873046875000000000000000000000e-01,
    3.0126133057816173455023545102449133992195129394531250000000000000000000e-01,
    3.0702503529491181888388950937951449304819107055664062500000000000000000e-01,
    3.1275571000389684739317885942000430077314376831054687500000000000000000e-01,
    3.1845373111853458869546784626436419785022735595703125000000000000000000e-01,
    3.2411946865421192853773391107097268104553222656250000000000000000000000e-01,
    3.2975328637246797969240219572384376078844070434570312500000000000000000e-01,
    3.3535554192113781191153520921943709254264831542968750000000000000000000e-01,
    3.4092658697059319283795275623560883104801177978515625000000000000000000e-01,
    3.4646676734620857063262633346312213689088821411132812500000000000000000e-01,
    3.5197642315717814209818925519357435405254364013671875000000000000000000e-01,
    3.5745588892180374385176833129662554711103439331054687500000000000000000e-01,
    3.6290549368936841911903457003063522279262542724609375000000000000000000e-01,
    3.6832556115870762614150635272380895912647247314453125000000000000000000e-01,
    3.7371640979358405898480555151763837784528732299804687500000000000000000e-01,
    3.7907835293496944251145919224654790014028549194335937500000000000000000e-01,
    3.8441169891033200034513583887019194662570953369140625000000000000000000e-01,
    3.8971675114002518602873692543653305619955062866210937500000000000000000e-01,
    3.9499380824086893770896722344332374632358551025390625000000000000000000e-01,
    4.0024316412701266276741307592601515352725982666015625000000000000000000e-01,
    4.0546510810816432934799991016916465014219284057617187500000000000000000e-01,
    4.1065992498526837639616360320360399782657623291015625000000000000000000e-01,
    4.1582789514371093497757669865677598863840103149414062500000000000000000e-01,
    4.2096929464412963239894338585145305842161178588867187500000000000000000e-01,
    4.2608439531090003260516141381231136620044708251953125000000000000000000e-01,
    4.3117346481837132143866142541810404509305953979492187500000000000000000e-01,
    4.3623676677491801667585491486534010618925094604492187500000000000000000e-01,
    4.4127456080487520440058801796112675219774246215820312500000000000000000e-01,
    4.4628710262841947420398014401143882423639297485351562500000000000000000e-01,
    4.5127464413945855836729492693848442286252975463867187500000000000000000e-01,
    4.5623743348158757315857769754074979573488235473632812500000000000000000e-01,
    4.6117571512217014895185229761409573256969451904296875000000000000000000e-01,
    4.6608972992459918316399125615134835243225097656250000000000000000000000e-01,
    4.7097971521879100631480241645476780831813812255859375000000000000000000e-01,
    4.7584590486996386493601107758877333253622055053710937500000000000000000e-01,
    4.8068852934575190261057286988943815231323242187500000000000000000000000e-01,
    4.8550781578170076890899053978500887751579284667968750000000000000000000e-01,
    4.9030398804519381705802061333088204264640808105468750000000000000000000e-01,
    4.9507726679785146739476431321236304938793182373046875000000000000000000e-01,
    4.9982786955644931126130359189119189977645874023437500000000000000000000e-01,
    5.0455601075239520092452494282042607665061950683593750000000000000000000e-01,
    5.0926190178980790257412536448100581765174865722656250000000000000000000e-01,
    5.1394575110223428282552049495279788970947265625000000000000000000000000e-01,
    5.1860776420804555186805373523384332656860351562500000000000000000000000e-01,
    5.2324814376454775732838697877014055848121643066406250000000000000000000e-01,
    5.2786708962084227803046587723656557500362396240234375000000000000000000e-01,
    5.3246479886947173376654518506256863474845886230468750000000000000000000e-01,
    5.3704146589688361856929077475797384977340698242187500000000000000000000e-01,
    5.4159728243274429804188230264117009937763214111328125000000000000000000e-01,
    5.4613243759813556721383065450936555862426757812500000000000000000000000e-01,
    5.5064711795266219063194057525834068655967712402343750000000000000000000e-01,
    5.5514150754050151093110798683483153581619262695312500000000000000000000e-01,
    5.5961578793542265941596269840374588966369628906250000000000000000000000e-01,
    5.6407013828480290218436721261241473257541656494140625000000000000000000e-01,
    5.6850473535266865532378233183408156037330627441406250000000000000000000e-01,
    5.7291975356178548306473885531886480748653411865234375000000000000000000e-01,
    5.7731536503482350219940144597785547375679016113281250000000000000000000e-01,
    5.8169173963462239562716149521293118596076965332031250000000000000000000e-01,
    5.8604904500357812846544902640744112432003021240234375000000000000000000e-01,
    5.9038744660217634674381770309992134571075439453125000000000000000000000e-01,
    5.9470710774669277576265358220553025603294372558593750000000000000000000e-01,
    5.9900818964608337768851242799428291618824005126953125000000000000000000e-01,
    6.0329085143808425240052883964381180703639984130859375000000000000000000e-01,
    6.0755525022454170969155029524699784815311431884765625000000000000000000e-01,
    6.1180154110599282990534675263916142284870147705078125000000000000000000e-01,
    6.1602987721551394351138242200249806046485900878906250000000000000000000e-01,
    6.2024040975185745772080281312810257077217102050781250000000000000000000e-01,
    6.2443328801189346144440150965237990021705627441406250000000000000000000e-01,
    6.2860865942237409420556559780379757285118103027343750000000000000000000e-01,
    6.3276666957103777644277897707070223987102508544921875000000000000000000e-01,
    6.3690746223706917739093569252872839570045471191406250000000000000000000e-01,
    6.4103117942093124081992527862894348800182342529296875000000000000000000e-01,
    6.4513796137358470073053240412264131009578704833984375000000000000000000e-01,
    6.4922794662510974195157587018911726772785186767578125000000000000000000e-01,
    6.5330127201274557080523663898929953575134277343750000000000000000000000e-01,
    6.5735807270835999727154330685152672231197357177734375000000000000000000e-01,
    6.6139848224536490484126716182800009846687316894531250000000000000000000e-01,
    6.6542263254509037562201001492212526500225067138671875000000000000000000e-01,
    6.6943065394262923906154583164607174694538116455078125000000000000000000e-01,
    6.7342267521216669923234121597488410770893096923828125000000000000000000e-01,
    6.7739882359180603188519853574689477682113647460937500000000000000000000e-01,
    6.8135922480790300781450241629499942064285278320312500000000000000000000e-01,
    6.8530400309891936760919861626462079584598541259765625000000000000000000e-01,
    6.8923328123880889251040571252815425395965576171875000000000000000000000e-01
};


double MPFR34Log1p(float f)
{
	mpfr_t x;
	mpfr_init2(x,200);
	mpfr_set_flt(x,f,MPFR_RNDN);
	int sticky = 0;
	int status = mpfr_log1p(x,x,MPFR_RNDZ);
	if(status!=0) sticky = 0x1;
	return FromMPFRToFloat34Ro(x, sticky);
}

void CalculateInterval(double x, double& lb, double& ub) {
	
	unsigned long binary = doubleTo34Bit(x);

	if((binary & 0x1UL) == 0)
	{
		printf("ci wrong\n");
		exit(0);
	}

	if((binary & 0x1FE0000000UL) == 0x1FE0000000UL)
	{
		printf("error 2\n");
		exit(0);
	}
	//x positive
	if((binary & 0x200000000UL) == 0) {
		unsigned long ubBinary = binary + 1UL;
		Double dx;
		dx.d = bin34ToDouble(ubBinary);
		dx.x--;
		ub = dx.d;

		unsigned long lbBinary = binary - 1UL;
		dx.d = bin34ToDouble(lbBinary);
		dx.x++;
		lb = dx.d;
	} else {
		unsigned long lbBinary = binary + 1UL;
		Double dx;
		dx.d = bin34ToDouble(lbBinary);
		dx.x--;
		lb = dx.d;

		unsigned long ubBinary = binary - 1UL;
		dx.d = bin34ToDouble(ubBinary);
		dx.x++;
		ub = dx.d;
	}
}

#endif
