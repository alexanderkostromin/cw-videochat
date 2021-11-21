#ifndef OfferDto_hpp
#define OfferDto_hpp
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

class OfferDto : public oatpp::DTO {

	DTO_INIT(OfferDto, DTO /* Extends */);

	DTO_FIELD(Int32 , id);
	DTO_FIELD(String, offer);
};

class RoomDto: public oatpp::DTO {
	DTO_INIT(RoomDto, DTO /* Extends */);
	DTO_FIELD(Int32 , id);
	DTO_FIELD(String, uid);
	DTO_FIELD(String, offer);
	DTO_FIELD(String, answer);
	DTO_FIELD(String, ice_candidate_offer);
	DTO_FIELD(String, ice_candidate_answer);
};

class AnswerDto : public oatpp::DTO {

	DTO_INIT(AnswerDto, DTO /* Extends */);

	DTO_FIELD(Int32 , id);
	DTO_FIELD(String, answer);
};

class CandidateDto : public oatpp::DTO {

	DTO_INIT(CandidateDto, DTO /* Extends */);

	DTO_FIELD(Boolean, creator);
	DTO_FIELD(Int32 , id);
	DTO_FIELD(String, ice_candidate);
};

class MessageDto : public oatpp::DTO {

	DTO_INIT(MessageDto, DTO /* Extends */);

	DTO_FIELD(String, message);
};
/* TODO - Add more DTOs here */

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */