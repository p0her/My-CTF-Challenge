from Crypto.Util.number import *

N1 = 22492674366996089747748865726869494034063110168081625491752605636520026900327672187969242054110766356344395149581026115760077148021207967966319512616083092113028324935363754922146964990633811114418466673457918765479409097322404200778099365238153869711544101419496064878407229058033101466917711732034419151569566908027480256124801157725784737154952591389412274668941467484692121176455580555795367264848053112557410976462457038952000583773580111064775046154286100347868036341119399123356212458455418152792511551232377293680254447115733131867677087404515352963155417520640299045424327526205575703010236003254672968330701
e1 = 15835
e2 = 34837
e3 = 65537
c1 = 15659454997175560139468472124968295053895554783145334071593528081976611854622220750860282378989527020196657182377912220942012065064676643937645407013727720588337418629879270092669514854009196223052683742561347386314580475447359082242730205531342976852347308398823564100072807405593241214005895751441018358497741583379358676573661836127283909850333887435279758507341430965782382750660897561772251002449578651283917931498763259535182046901471956261582265368221877000880772385578344533247645733145340802183747127697839396461970953306404509115800197094824218070256086213965858860479754894285813234092356326350070592183096
c2 = 5659035702360038717351647878113607334108977354536934370182333489874800136861738411663330685207848730636257063521987567070397142127348244540560294604689558477696398575416882690285939320333692190346045091934327190317658823914250226565675654377597909143101577295778940138896190871468636210480900383917572942794553885097774969998293814952731314043264270739970746204249873350535894357319868949596573465774371938319537518295722673144504754171026015712507381720074074763132688111967648623335592943418789882587775078220049486893097786554208488022401009325506228564356629611937760681022704444366729086315401853069933252312051
c3 = 8750136751452553085117938869398162928623208053214335556597269958743500619574454836370086126460668474966410421527756926051173572797597730179425235764198949622742024868391361502034820147691257523159236342902023733246254616837095000301227687094601839697137945581808889328239938168079224507032102262172286730544744161697098506283205398013903142748169169173595746604772194391824677227336710343601592551904360140058403530836760895722865610825778062788958399542551032042658552744390587413286709671352108492163650430302184702269478205014519192950189618609721766140377147289284317585450600942419601661867117367453963369577020

def extended_gcd(a, b):
    if a == 0:
        return (0, 1, b)
    else:
        x1, y1, gcd = extended_gcd(b % a, a)
        x = y1 - (b // a) * x1
        y = x1
        return (x, y, gcd)

def find_xyz(a, b, c):
    x1, y1, d = extended_gcd(a, b)
    if d == 1:
        x, y, _ = x1, y1, d
    else:
        a //= d
        b //= d
        kx, ky, _ = extended_gcd(a, b)
        
        for k in range(1, c):
            if (d * k) % c == 1:
                break
        x, y = k * kx, k * ky
    z = (1 - d * (a * x + b * y)) // c
    return (x, y, z)

x, y, z = find_xyz(e1, e2, e3)

flag = (pow(c1, x, N1) * pow(c2, y, N1) * pow(c3, z, N1)) % N1

print(long_to_bytes(flag).decode())
