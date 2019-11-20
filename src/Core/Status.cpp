// Status.cpp

#include "Status.h"

#include <functional>

namespace dbbenchmark {
namespace utility {

Status Status::OK("OK", "The operation completed successfully.");
Status Status::ERROR("ERROR", "The operation failed.");
Status Status::NOT_FOUND("NOT_FOUND", "The requested record was not found.");
Status Status::NOT_IMPLEMENTED("NOT_IMPLEMENTED", "The operation is not"
    " implemented for the current binding.");
Status Status::UNEXPECTED_STATE("UNEXPECTED_STATE", "The operation reported"
    " success, but the result was not as expected.");
Status Status::BAD_REQUEST("BAD_REQUEST", "The request was not valid.");
Status Status::FORBIDDEN("FORBIDDEN", "The operation is forbidden.");
Status Status::SERVICE_UNAVAILABLE("SERVICE_UNAVAILABLE", "Dependant"
    " service for the current binding is not available.");
Status Status::BATCHED_OK("BATCHED_OK", "The operation has been batched by"
    " the binding to be executed later.");

Status::Status(const std::string &nameIn, const std::string &descriptionIn) : name(nameIn),
description(descriptionIn) {
}

std::string Status::toString() const {
    std::string tmp;
    tmp = "Status [name=" + name + ", description=" + description + "]";
    return tmp; 
};

int Status::hashCode() const {
    const int prime = 31;
    int result = 1;
    result = ((prime * result) + (description.empty())) ? 0 : std::hash<std::string>{}(description);
    result = ((prime * result) + (name.empty())) ? 0 : std::hash<std::string>{}(name);
    return result;
}

bool Status::isEqual(const Status& statusToCompare) const {
    return ((this->description == statusToCompare.description) && (this->name == statusToCompare.name));
}

} // namespace utility
} // namespace dbbenchmark
