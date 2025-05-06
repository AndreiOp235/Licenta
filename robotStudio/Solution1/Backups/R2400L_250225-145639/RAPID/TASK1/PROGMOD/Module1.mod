MODULE Module1
    ! Define constants for movement positions
    CONST robtarget Target_10:=[[818.804543143,0,1822.307621556],[0.499999994,0,0.866025407,0],[0,0,0,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];
    CONST robtarget Target_20:=[[928.612158702,-400,1412.499998927],[0.499999994,0,0.866025407,0],[0,0,0,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];

    ! Define variables
    VAR socketdev server;
    VAR socketdev client;
    VAR string message;
    VAR rawbytes data;
    VAR bool move_enabled := FALSE;  ! Flag to control movement

    !***********************************************************
    ! Module:  Module1
    ! Description:
    !   Handles socket communication and runs movement asynchronously.
    !***********************************************************

    PROC main()
        ! Create and bind the server socket
        SocketCreate server;
        SocketBind server,"127.0.0.1",5509;
        SocketListen server;
        SocketAccept server,client;

        ! Send initial message
        SocketSend client, \Str:="buna seara!";
        
        ! Main loop to listen for commands
        WHILE TRUE DO
            SocketReceive client, \RawData:=data;
            UnpackRawBytes data,1, message \ASCII:=1;

            ! Check received command
            IF message = "S" THEN
                Path_10;
            ELSEIF message = "R" THEN
                SocketClose client;
                SocketClose server;
                Stop;
            ENDIF
        ENDWHILE
    ENDPROC

    PROC Path_10()

                MoveL Target_10,v1000,z100,tool0\WObj:=wobj0;
                MoveL Target_20,v1000,z100,tool0\WObj:=wobj0;
                MoveL Target_10,v1000,z100,tool0\WObj:=wobj0;
    ENDPROC
ENDMODULE
