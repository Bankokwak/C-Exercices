le parent modifier à l'adresse donné le premier octet(et le save) en 0xCC (INT3), je laisse l'enfant continuer avec ptrace CONT, le programme fonctionne normalment et quand il arrive à INT3 il s'arrete et je recois un SIGTRAP, et la je peux faire ce que je veux, puis je dois apres cela modifier l'ocet 0xCC en ce que j'ai save en reculant RIP (d'un octet), et je refait CONT


char stringToSplit[64] = "Bonjour|@,je suis,absent,meurs|,tktdqsdqdzddq|,@ddqdadd45dqsdqd";
    
    int maxResult = 3;
    int maxCharResult = 64;
    char result[maxResult][maxCharResult] = {};
    int resultCount = SplitString(stringToSplit, "|,@", maxResult, maxCharResult, result);

    for(int i = 0; resultCount > i; i++){
        printf("%i: %s\n", i, result[i]);
    }