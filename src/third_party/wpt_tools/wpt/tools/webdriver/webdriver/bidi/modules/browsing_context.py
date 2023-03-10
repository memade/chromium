import base64
from typing import Any, Optional, Mapping, MutableMapping

from ._module import BidiModule, command


class BrowsingContext(BidiModule):
    @command
    def capture_screenshot(self, context: str) -> Mapping[str, Any]:
        params: MutableMapping[str, Any] = {
            "context": context
        }

        return params

    @capture_screenshot.result
    def _capture_screenshot(self, result: Mapping[str, Any]) -> bytes:
        assert result["data"] is not None
        return base64.b64decode(result["data"])

    @command
    def close(self, context: Optional[str] = None) -> Mapping[str, Any]:
        params: MutableMapping[str, Any] = {}

        if context is not None:
            params["context"] = context

        return params

    @command
    def create(self, type_hint: str, reference_context: Optional[str] = None) -> Mapping[str, Any]:
        params: MutableMapping[str, Any] = {"type": type_hint}

        if reference_context is not None:
            params["referenceContext"] = reference_context

        return params

    @create.result
    def _create(self, result: Mapping[str, Any]) -> Any:
        assert result["context"] is not None

        return result

    @command
    def get_tree(self,
                 max_depth: Optional[int] = None,
                 root: Optional[str] = None) -> Mapping[str, Any]:
        params: MutableMapping[str, Any] = {}

        if max_depth is not None:
            params["maxDepth"] = max_depth
        if root is not None:
            params["root"] = root

        return params

    @get_tree.result
    def _get_tree(self, result: Mapping[str, Any]) -> Any:
        assert result["contexts"] is not None
        assert isinstance(result["contexts"], list)

        return result["contexts"]

    @command
    def navigate(
        self, context: str, url: str, wait: Optional[str] = None
    ) -> Mapping[str, Any]:
        params: MutableMapping[str, Any] = {"context": context, "url": url}
        if wait is not None:
            params["wait"] = wait
        return params

    @navigate.result
    def _navigate(self, result: Mapping[str, Any]) -> Any:
        if result["navigation"] is not None:
            assert isinstance(result["navigation"], str)

        assert result["url"] is not None
        assert isinstance(result["url"], str)

        return result
