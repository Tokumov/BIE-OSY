#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <cmath>
#include <cfloat>
#include "sample_tester.h"

class CTestData
{
  public:
    APolygon            m_Polygon;
    double              m_TriangMin;
    const char        * m_TriangCnt;
};
//=============================================================================================================================================================
std::initializer_list<CTestData> g_Data =
{
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{0,0},{10,10},{25,10},{40,0}} ), 114.095716037, "2" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{0,0},{0,10},{10,10},{10,0}} ), 54.1421356237, "2" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{0,10},{10,20},{20,20},{30,10},{20,0},{10,0}} ), 138.92922227, "14" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{0,0},{5,100},{10,0},{5,98}} ), 398.504780189, "1" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{10000,0},{6544,7561},{2550,9669},{-5210,8535},{-9794,2016},{-7526,-6584},{-5428,-8398},{-1707,-9853},{5817,-8133},{8256,-5641}} ), 153417.876879, "1430" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{2870,-201},{6334,1403},{2688,-2184},{1334,-2207},{2254,-2782},{-3498,-9044},{415,-3534},{0,0},{0,10000},{10000,5000}} ), 84367.7711704, "13" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{-5054,6201},{7967,6042},{5285,8488},{-1139,9934},{-6318,7751},{-9411,3380},{-8098,-5866},{-5497,-8353},{2075,-9782},{7039,-7102}} ), 139509.911946, "168" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{9457,0},{5190,3771},{2089,6431},{-2792,8595},{-7762,5639},{-5348,0},{-7884,-5728},{-2357,-7255},{1677,-5164},{4822,-3503}} ), 115880.657052, "737" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{10000,0},{9165,4000},{5867,8097},{3733,9276},{2854,9584},{2179,9759},{-824,9965},{-1742,9847},{-3530,9356},{-8050,5932},{-9953,963},{-9151,-4030},{-8164,-5774},{-5722,-8200},{-3436,-9390},{1120,-9937},{4292,-9031},{7563,-6542},{9317,-3630},{9577,-2875}} ), 210729.014328, "477638700" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{7522,2981},{7323,2272},{5963,117},{7590,3310},{6701,2622},{6812,2574},{4942,1038},{2519,-923},{3677,-86},{5011,392},{2114,-3000},{2120,-1648},{-2209,-4775},{1237,-1717},{5086,1365},{5011,1646},{4278,1465},{0,0},{0,10000},{10000,5000}} ), 112268.27949, "14208" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{-4709,6466},{9593,2822},{7830,6219},{6388,7693},{2184,9758},{-3,9999},{-2902,9569},{-4547,8905},{-5887,8083},{-7403,6722},{-9266,3759},{-9942,1071},{-9991,-405},{-8381,-5454},{-7713,-6363},{-6860,-7275},{-4226,-9062},{228,-9997},{5649,-8251},{9134,-4070}} ), 203151.112553, "36469158" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{8748,0},{6508,2114},{5234,3802},{3621,4984},{2448,7534},{0,6222},{-1780,5479},{-5090,7006},{-4552,3307},{-8528,2770},{-5820,0},{-8297,-2696},{-4346,-3157},{-5730,-7887},{-1626,-5004},{0,-7607},{1817,-5593},{3644,-5015},{7271,-5283},{8239,-2677}} ), 161825.6862, "466116" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{10000,0},{9588,2840},{7695,6385},{5618,8272},{2666,9637},{1919,9814},{1007,9949},{-1355,9907},{-2055,9786},{-2714,9624},{-3833,9236},{-5713,8207},{-8176,5756},{-9370,3491},{-9995,315},{-9995,-296},{-9914,-1301},{-8539,-5204},{-7865,-6175},{-5051,-8630},{-4266,-9044},{-248,-9996},{371,-9993},{3944,-9189},{4566,-8896},{4946,-8690},{7867,-6173},{9207,-3902},{9578,-2873},{9822,-1875}} ), 246179.920552, "263747951750360" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{3479,-1348},{-2368,-7179},{-8622,-14691},{-848,-4622},{-844,-4472},{-899,-4625},{-972,-4746},{-986,-4764},{-991,-4798},{-1109,-4644},{-1423,-5196},{-1404,-5186},{-1518,-5334},{-1678,-5624},{-1385,-4992},{-1956,-5732},{-2038,-5844},{-2056,-5565},{-3468,-7408},{-2824,-6261},{-4313,-7949},{-3711,-7061},{1914,-1566},{3721,444},{1683,-1597},{1395,-1708},{4137,1296},{0,0},{0,10000},{10000,5000}} ), 154188.748303, "5400" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{-6427,-4762},{9649,2626},{8720,4894},{8354,5496},{6795,7336},{5621,8270},{3225,9465},{86,9999},{-2983,9544},{-5631,8263},{-6392,7689},{-8237,5669},{-9199,3920},{-9613,2753},{-9995,-292},{-9431,-3324},{-8034,-5953},{-6803,-7328},{-5139,-8578},{-4409,-8975},{-3334,-9427},{-439,-9990},{1386,-9903},{3083,-9512},{5685,-8226},{7095,-7046},{7900,-6130},{8410,-5409},{9459,-3243},{9971,-758}} ), 255454.852922, "7915812652868" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{7462,0},{8989,1910},{7875,3506},{4765,3462},{4087,4539},{3746,6488},{1732,5331},{594,5659},{-711,6764},{-2046,6299},{-4829,8364},{-6384,7091},{-6561,4766},{-8390,3735},{-8891,1889},{-7480,0},{-6528,-1387},{-6368,-2835},{-6709,-4875},{-5382,-5977},{-3147,-5450},{-1778,-5474},{-560,-5332},{578,-5508},{2545,-7832},{3408,-5903},{4309,-4786},{5384,-3912},{4983,-2218},{6980,-1483}} ), 191449.597334, "13894103427" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{10000,0},{9654,2604},{9053,4245},{8610,5084},{7469,6649},{5838,8118},{4552,8903},{3940,9190},{1538,9880},{-311,9995},{-1884,9820},{-2666,9637},{-4245,9054},{-5428,8398},{-7249,6887},{-8065,5911},{-8562,5166},{-9078,4193},{-9756,2191},{-9994,320},{-9703,-2416},{-9368,-3497},{-8187,-5741},{-7814,-6239},{-6928,-7210},{-6714,-7410},{-4888,-8723},{-2836,-9589},{-1845,-9828},{-1284,-9917},{1170,-9931},{3212,-9470},{3591,-9332},{4869,-8734},{7069,-7072},{8368,-5474},{8784,-4778},{9391,-3436},{9945,-1045},{9985,-530}} ), 276721.493614, "176733862787006701400" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{8800,3797},{8536,3520},{8344,3233},{8036,2545},{8512,4093},{8235,3932},{7684,3647},{6138,2746},{2303,510},{5873,2473},{2207,300},{4235,1460},{8320,3956},{7218,3276},{4915,1873},{6968,3021},{7748,3203},{6547,2150},{6473,1492},{4836,-906},{6244,2287},{2552,-388},{5094,1132},{3934,64},{4033,-966},{2313,-2685},{2043,-2956},{1342,-3658},{1216,-3783},{1484,-3515},{1894,-3105},{1137,-3861},{1542,-3470},{151,-4946},{2998,-1777},{1025,-2680},{797,-2203},{0,0},{0,10000},{10000,5000}} ), 161469.645978, "3636350" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{-2419,-7625},{9993,372},{9975,692},{9726,2323},{9407,3391},{8407,5414},{6754,7373},{5403,8414},{3996,9166},{3512,9362},{2720,9622},{1111,9938},{-1733,9848},{-2151,9765},{-2675,9635},{-3607,9326},{-5613,8275},{-7141,6999},{-7524,6586},{-9118,4104},{-9883,1522},{-9970,762},{-9963,-855},{-9475,-3196},{-8415,-5401},{-8234,-5673},{-7012,-7129},{-5168,-8560},{-3024,-9531},{-219,-9997},{353,-9993},{2796,-9601},{5440,-8390},{5923,-8056},{6746,-7381},{7428,-6695},{8872,-4612},{9120,-4100},{9828,-1846},{9969,-784}} ), 283627.642094, "4596260132892984576" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{8804,0},{5415,857},{9420,3060},{8220,4188},{5426,3942},{5983,5983},{4280,5891},{3255,6389},{2555,7865},{1035,6537},{0,8002},{-883,5579},{-1670,5141},{-3127,6138},{-3340,4598},{-5868,5868},{-4507,3275},{-8378,4268},{-7524,2444},{-5979,947},{-8341,0},{-7549,-1195},{-6239,-2027},{-8657,-4410},{-7152,-5196},{-3966,-3966},{-3051,-4199},{-3438,-6748},{-2161,-6653},{-947,-5982},{0,-7263},{907,-5728},{2021,-6220},{3254,-6386},{2954,-4066},{5832,-5832},{4555,-3309},{6512,-3318},{5154,-1674},{8791,-1392}} ), 227916.968755, "130919653130" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{10000,0},{9849,1730},{9466,3224},{8646,5023},{7544,6563},{7318,6814},{7106,7035},{6183,7859},{5734,8192},{3979,9174},{3583,9336},{2664,9638},{1321,9912},{-128,9999},{-1693,9855},{-2920,9564},{-3864,9223},{-5461,8376},{-6528,7574},{-7904,6125},{-8283,5602},{-9130,4078},{-9473,3202},{-9735,2283},{-9820,1887},{-9999,116},{-9963,-848},{-9685,-2487},{-8986,-4386},{-8018,-5974},{-6799,-7332},{-5543,-8322},{-4285,-9035},{-3157,-9488},{-1776,-9841},{-595,-9982},{741,-9972},{2241,-9745},{2506,-9680},{4090,-9125},{5305,-8476},{5710,-8209},{6109,-7916},{6569,-7539},{7764,-6301},{8792,-4763},{9452,-3262},{9624,-2716},{9922,-1239},{9991,-420}} ), 297808.562838, "131327898242169365477991900" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{9628,4620},{9538,4501},{9644,4717},{9402,4495},{9252,4222},{9114,3968},{9254,4515},{8912,4245},{8874,4021},{8317,4059},{6809,3245},{7442,3567},{5350,2478},{8021,3826},{7096,3253},{5364,2239},{7436,3407},{8587,4005},{8985,4013},{8712,3877},{8070,3070},{7991,2992},{8296,3290},{7953,2914},{7361,2194},{8587,3747},{7941,2939},{7485,2475},{7339,2754},{6943,1560},{7353,3190},{6809,2896},{5242,2020},{6373,2150},{6740,1741},{6398,1411},{5942,1079},{3898,-1952},{5478,1735},{4032,14},{2240,-1439},{2305,-2409},{-3031,-8603},{623,-3257},{431,-1914},{3354,483},{4373,1916},{0,0},{0,10000},{10000,5000}} ), 156287.395566, "4487683200" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{-7378,3090},{9927,1206},{9751,2216},{9356,3528},{8722,4891},{7916,6110},{7303,6830},{6416,7669},{5888,8082},{4298,9028},{3990,9169},{2295,9732},{1416,9899},{1093,9939},{-812,9966},{-1761,9843},{-2937,9558},{-3429,9393},{-4261,9046},{-6190,7853},{-7480,6636},{-7697,6383},{-8931,4497},{-9223,3862},{-9675,2526},{-9913,1311},{-9992,393},{-9919,-1268},{-9662,-2576},{-9167,-3993},{-8322,-5544},{-7482,-6633},{-7185,-6954},{-6814,-7319},{-6460,-7632},{-5064,-8622},{-3480,-9374},{-2341,-9722},{-1694,-9855},{496,-9987},{1262,-9920},{2023,-9793},{3663,-9304},{4957,-8684},{5692,-8221},{6746,-7381},{8175,-5759},{9096,-4154},{9652,-2612},{9829,-1840}} ), 302019.025454, "2731171325575404706226856" },
  { std::make_shared<CPolygon> ( std::initializer_list<CPoint>{{6639,0},{8855,1118},{8119,2084},{8862,3508},{5603,3080},{4108,2984},{5864,5507},{5798,7008},{4390,6917},{2899,6161},{2132,6564},{1631,8550},{472,7511},{-470,7485},{-1374,7206},{-1934,5952},{-4195,8915},{-4497,7087},{-3872,4680},{-4502,4227},{-7866,5715},{-8503,4674},{-8487,3360},{-6093,1564},{-7383,932},{-8863,0},{-5218,-659},{-5708,-1465},{-4875,-1930},{-8083,-4444},{-6574,-4776},{-5017,-4711},{-5195,-6280},{-3488,-5497},{-2731,-5804},{-2950,-9079},{-1234,-6472},{-594,-9442},{542,-8626},{1833,-9609},{1938,-5965},{2361,-5018},{4548,-7166},{5608,-6778},{5865,-5507},{4712,-3423},{4432,-2436},{7345,-2908},{8929,-2292},{6085,-768}} ), 269897.999129, "1289809658727556" }
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
bool                                   smallDiff                               ( double                                x,
                                                                                 double                                ref )
{
  return std::fabs ( x - ref ) <= 1e8 * DBL_EPSILON * std::fabs ( ref );
}
//=============================================================================================================================================================
AProblemPack                           CCompanyTest::waitForPack               ()
{
  if ( m_MinPos > g_Data . size ()
       || m_CntPos > g_Data . size () )
    throw std::invalid_argument ( "waitForPack: called too many times" );

  if ( m_MinPos == g_Data . size ()
       && m_CntPos == g_Data . size () )
    return AProblemPack ();

  size_t nMin = std::min<size_t> ( rand () % 4 + 1, g_Data . size () - m_MinPos );
  size_t nCnt = std::min<size_t> ( rand () % 4 + 1, g_Data . size () - m_CntPos );
  AProblemPack res = std::make_shared<CProblemPack> ();
  while ( nMin -- )
    res -> addMin ( g_Data . begin () [m_MinPos ++] . m_Polygon );
  while ( nCnt -- )
    res -> addCnt ( g_Data . begin () [m_CntPos ++] . m_Polygon );
  return res;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
void                                   CCompanyTest::solvedPack                ( AProblemPack                          pack )
{
  for ( auto p : pack -> m_ProblemsMin )
  {
    size_t idx = m_MinDone ++;

    if ( idx >= g_Data . size () )
      throw std::invalid_argument ( "solvedPack: called too many times" );

    if ( g_Data . begin () [idx] . m_Polygon != p )
      throw std::invalid_argument ( "solvedPack: order not preserved" );

    if ( ! smallDiff ( p -> m_TriangMin, g_Data . begin () [idx] . m_TriangMin ) )
      throw std::invalid_argument ( "solvedPack: invalid result (TriangMin)" );
  }

  for ( auto p : pack -> m_ProblemsCnt )
  {
    size_t idx = m_CntDone ++;

    if ( idx >= g_Data . size () )
      throw std::invalid_argument ( "solvedPack: called too many times" );

    if ( g_Data . begin () [idx] . m_Polygon != p )
      throw std::invalid_argument ( "solvedPack: order not preserved" );

    if ( p -> m_TriangCnt != CBigInt ( g_Data . begin () [idx] . m_TriangCnt ) )
      throw std::invalid_argument ( "solvedPack: invalid result (TriangCnt)" );
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
bool                                   CCompanyTest::allProcessed              ( void ) const
{
  return m_MinPos == g_Data . size ()
         && m_MinDone == g_Data . size ()
         && m_CntPos == g_Data . size ()
         && m_CntDone == g_Data . size ();
}
//=============================================================================================================================================================
