semaphore_wait(semaphore); //
        
        printf("    PID %d enters shared section number %d\n",getpid(),i+1);
        
        fd=open(argv[2],O_RDONLY);
        if(fd==-1)
        {
            perror("Open error");
            _exit(12);
        }
        
        r = read(fd,count,5);        
        if(r==-1)
        {
            perror("Read error");
            _exit(10);
        }
        
        printf("    PID %d takes from file : %s\n",getpid(),count);
        
        if(close(fd)==-1)
        {
            perror("Close error");
            _exit(11);
        }
        
        number=atoi(count);
        number++;
        
        if(number==10||number==100||number==1000||number==10000)
            r++;
        sprintf(count,"%d",number);
        
        printf("    PID %d writes into file : %s\n",getpid(),count);
        printf("    Semaphore value = %d\n",semaphore_value(semaphore)); //
        
        usleep((rand()%10+1)*100000);
        
        fd=open(argv[2],O_WRONLY,O_TRUNC);
        if(fd==-1)
        {
            perror("Open error");
            _exit(8);
        }
        if(write(fd,count,r)==-1)
        {
            printf("Write error");
            _exit(13);
        }
        semaphore_post(semaphore); //
