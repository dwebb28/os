struct rpc_data{
	char * content;
};

program COUNT_NUMBERS{
	version COUNT_VERS{
		int count(rpc_data)=1;
	}=1;
}=0x12344;