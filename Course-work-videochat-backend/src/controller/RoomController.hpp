#ifndef RoomController_hpp
#define RoomController_hpp

#include "dto/OfferDto.hpp"
#include "db/DbClient.hpp"
#include "service/RoomService.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"


#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include <iostream>
#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen



class RoomController : public oatpp::web::server::api::ApiController {
private:
	RoomService roomService;
public:
	RoomController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
			: oatpp::web::server::api::ApiController(objectMapper)
	{}

	ENDPOINT_INFO(getRoom) {
		info->summary = "Get information about room";
		info->addResponse<Object<RoomDto>>(Status::CODE_200, "application/json");
	}
	ADD_CORS(getRoom)
	ENDPOINT("GET", "get_room", getRoom, QUERY(Int32, id)) {
		return createDtoResponse(Status::CODE_200, roomService.getById(id));
	}

	ENDPOINT_INFO(createRoom) {
		info->summary = "Create new room";
		info->addResponse<Object<RoomDto>>(Status::CODE_200, "application/json");
	}
	ADD_CORS(createRoom, "*", "PUT")
	ENDPOINT("POST", "create_room", createRoom) {
		return createDtoResponse(Status::CODE_201, roomService.createRoom());
	}

	ENDPOINT_INFO(setAnswer) {
		info->summary = "Update answer";
		info->addConsumes<Object<AnswerDto>>("application/json");
		info->addResponse<Object<RoomDto>>(Status::CODE_200, "application/json");
	}
	ADD_CORS(setAnswer, "*", "PUT")
	ENDPOINT("PUT", "set_answer", setAnswer, BODY_DTO(Object<AnswerDto>, answerDto)) {
		return createDtoResponse(Status::CODE_200, roomService.setAnswer(answerDto));
	}

	ENDPOINT_INFO(setOffer) {
		info->summary = "Update offer";
		info->addConsumes<Object<OfferDto>>("application/json");
		info->addResponse<Object<RoomDto>>(Status::CODE_200, "application/json");
	}
	ADD_CORS(setOffer, "*", "PUT")
	ENDPOINT("PUT", "set_offer", setOffer, BODY_DTO(Object<OfferDto>, offerDto)) {
		return createDtoResponse(Status::CODE_200, roomService.setOffer(offerDto));
	}

	ENDPOINT_INFO(setIceCandidate) {
		info->summary = "Update ice candidate";
		info->addConsumes<Object<CandidateDto>>("application/json");
		info->addResponse<Object<RoomDto>>(Status::CODE_200, "application/json");
	}
	ADD_CORS(setIceCandidate, "*", "PUT")
	ENDPOINT("PUT", "set_ice_candidate", setIceCandidate, BODY_DTO(Object<CandidateDto>, candidateDto)) {
		return createDtoResponse(Status::CODE_200, roomService.setIceCandidate(candidateDto));
	}
};

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen

#endif /* RoomController_hpp */