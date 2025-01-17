/*

										DirectX 11의 랜더링 파이프라인의 개요
	
	========================================================================================================================
	기하단계										래스터라이즈
	========================================================================================================================
	IA						Vertex Shading			|	Pixel Shading					Output Merger
	========================================================================================================================

	Input
	Assembler				Vertex Shader			|	Pixel Shader					프레임 버퍼들을 최종적으로 병합


	다루는 데이터들 :
	VB
	IB
	vertex layout
	topology																			Depth/Stencil Buffer
	========================================================================================================================
							|							texture buffer				|
							|							constant buffer				|
	========================================================================================================================

	Shader SourceCode File --> 컴파일 --> 바이트 코드 ByteCode(중간 결과물)

*/